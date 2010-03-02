/** 
    \file event-statistics.hh 
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

#ifndef __EVENT_STATISTICS_HH
#define __EVENT_STATISTICS_HH

/// This class provides statistics for events spaced in time.
class EventStatistics {
  QQueue<long> events;

  /// The maximum time between the first and last events
  long frame;
public:

  /// Creates an EventStatistics object.
  EventStatistics(long f = 100000) : frame(f) {;};

  /// Add one event.
  ///
  /// \arg time is the time of the event (with respect to an arbitraty
  /// reference) in milliseconds
  void addEvent(long time);

  /// The number of events currently in the processing queue.
  int nbEvents() { return events.size();};

  /// Returns the rate of events between two times
  double averageRate(long t1, long t2);

  /// Returns the number of events per second over the whole frame.
  double averageRate(long currentTime);

  /// Divides the frame into nb segments and count the key strokes
  /// inside them. 
  QVector<long> histogram(int nb, long currentTime);

  /// Draws a histogram into a QPixmap and returns it.
  QPixmap history(long currentTime);

  
};

#endif
