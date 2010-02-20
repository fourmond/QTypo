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

class XRecordGather : public QObject {
  Q_OBJECT;
  /// The file descriptor of the pipe from the gathering thread.
  int gatheringFd;
public:
  XRecordGather() {;};
  ~XRecordGather() {;};

  /// Starts gathering on the given display (NULL means default) 
  int startGathering(const char * display = NULL);

};

#endif
