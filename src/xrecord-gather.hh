/** 
    \file xrecord-gather.hh 
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

#ifndef __XRECORD_GATHER_HH
#define __XRECORD_GATHER_HH

#include <event-statistics.hh>

/// The class gathering the information from the X server.
///
/// \todo the handling of time is shaky in that it does not update
/// when there are no events.
class XRecordGather : public QObject {
  Q_OBJECT;
  /// The file descriptor of the pipe from the gathering thread.
  int gatheringFd;


  /// Pulls data from the subthread and adds it to keyPressEvents.
  void pullFromGatheringThread();

  /// Returns true if the pipe has pending data
  bool pendingData();

  /// The timer handling the gathering of the data.
  QTimer refreshTimer;

  /// The subprocess PID
  int childPID;

  /// The precise time when the first event was received
  QDateTime firstEvent;

  /// The queue handling the actual compilation of the statistics. It
  /// is fed numbers in milliseconds from the first event received (ie
  /// the "Starting to record" event)
  EventStatistics keyPressEvents;
  
		      
  /// The server time of the first event.
  long firstServerTime;
protected slots:
  /// internally called by the timer to pull the data
  void doPullData();

public:
  /// Initializes a XRecordGather instance.
  ///
  /// \arg refreshRate the rate at which data is pulled from the
  /// underlying gathering process, in milliseconds.
  XRecordGather(int refreshRate = 1000);
  ~XRecordGather() {;};

  /// Starts gathering on the given display (NULL means default) 
  int startGathering(const char * display = NULL);

  /// The number of milliseconds since the first event
  long currentTime();

  EventStatistics * events() { return &keyPressEvents;};

signals:
  /// This signal is emitted whenever the state of the queue has
  /// changed, once for each call to pullFromGatheringThread().
  void newEvents();
};

#endif
