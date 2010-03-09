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

  /// The total number of events since the first one (included).
  int allTimeNumber;

  /// The maximum rate in 1 second
  double allTimeMaxRate;

  /// Creates an EventStatistics object.
  EventStatistics(long f = 400000) : frame(f), allTimeNumber(0), 
				     allTimeMaxRate(0) {;};

  /// Add one event.
  ///
  /// \arg time is the time of the event (with respect to an arbitraty
  /// reference) in milliseconds
  void addEvent(long time);

  /// The number of events currently in the processing queue.
  int nbEvents() { return events.size();};

  /// Returns the rate of events between two times
  double averageRate(long t1, long t2);

  /// Returns the maximum of the average over dt between t1 and t2.
  double movingAverage(long t1, long t2, long dt);

  /// Returns the moving average over the last dt*10 time.
  double movingAverage(long current, long dt) {
    return movingAverage(current - 10*dt, current, dt);
  };

  /// Returns the number of events per second over the whole frame.
  double averageRate(long currentTime);

  /// Divides the frame into nb segments and count the key strokes
  /// inside them. 
  QVector<long> histogram(int nb, long currentTime);

  /// Draws a histogram into a QPixmap and returns it.
  QPixmap history(long currentTime);

  // /// Returns the total number of events
  // int numberAllEvents() { return totalNumber;};

  /// The maximum rate during the period covered by the events queue.
  double maxRate();
};

#endif
