/* 
    xrecord-gather.cc 
    Copyright 2010 by Vincent Fourmond

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <headers.hh>
#include <xrecord-gather.hh>

#include <unistd.h>

#include <sys/select.h>
#include <stdio.h>

/// This local struct is used for passing around data along the pipe.
typedef struct {
  Time time;
} EventData;

static void intercept(int fd, XRecordInterceptData * data)
{
  EventData d;
  if(data->category == XRecordFromServer || 
	data->category == XRecordStartOfData ) {
    d.time = data->server_time;
    write(fd, &d, sizeof(d));
  }
  XRecordFreeData(data);
}

int XRecordGather::startGathering(const char * display)
{
  int fd[2];
  pipe(fd);
  if((childPID = fork())) {
    close(fd[1]);
    gatheringFd = fd[0];
    return 1;
  }
  close(fd[0]);
  Display * dpy = XOpenDisplay(display);
  if(! dpy) {
    fprintf(stderr, "Error opening display\n");
    exit(1);
  }

  int major, minor;
  if(! XRecordQueryVersion(dpy, &major, &minor)) {
    fprintf(stderr, "Error: xrecord does not seem to be supported\n");
    exit(1);
  }

  /* Allocating a single XRecordRange object */
  XRecordRange * range = XRecordAllocRange();
  if(! range) {
    fprintf(stderr, "Error: failed to allocate the record range\n");
    exit(1);
  }
  
  /* We only want KeyPress events to be recorded */
  range->device_events.first = KeyPress;
  range->device_events.last = KeyPress;
  XRecordClientSpec clientSpec = XRecordAllClients;
  XRecordContext context = 
    XRecordCreateContext(dpy, XRecordFromServerTime | XRecordFromClientTime,
                         &clientSpec, 1, &range, 1);
  if(! context) {
    fprintf(stderr, "Error: failed to create the context\n");
    exit(1);
  }

  /* Now, enabling the record */
  if( ! XRecordEnableContext(dpy, context, 
			     (void (*)(char*, XRecordInterceptData*)) 
			     intercept, 
			     (XPointer) fd[1])) {
    fprintf(stderr, "Error: failed to allocate the record range\n");
    exit(1);
  }
  
  exit(0);
}

bool XRecordGather::pendingData()
{
  fd_set rfds;
  struct timeval tv;
  int retval;
  
  FD_ZERO(&rfds);
  FD_SET(gatheringFd, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  retval = select(gatheringFd + 1, &rfds, NULL, NULL, &tv);
  if(retval < 0) {
    perror("select");
  }
  else if(retval > 0)
    return true;
  return false;
}

void XRecordGather::pullFromGatheringThread()
{
  EventData d;
  /// This clearly would need buffering, but I'm unsure if it would
  /// work fine. Anyway, the load should not be huge.
  int i = 0;
  while(pendingData()) {
    read(gatheringFd, &d, sizeof(d));
    if(firstEvent.isValid())
      keyPressEvents.addEvent(d.time - firstServerTime);
    else {
      firstEvent = QDateTime::currentDateTime();
      firstServerTime = d.time;
    }
    i++;
  }
  if(i)
    emit newEvents();
}

XRecordGather::XRecordGather(int refreshRate)
{
  refreshTimer.setSingleShot(false);
  refreshTimer.start(refreshRate);
  connect(&refreshTimer, SIGNAL(timeout()), SLOT(doPullData()));
  
}

void XRecordGather::doPullData()
{
  pullFromGatheringThread();
}


long  XRecordGather::currentTime()
{
  QDateTime cur = QDateTime::currentDateTime();
  return 
    firstEvent.secsTo(cur) * 1000 + 
    ( cur.time().msec() > firstEvent.time().msec() ? 
      cur.time().msec() - firstEvent.time().msec() : 
      1000 + cur.time().msec() - firstEvent.time().msec());
}
