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
#include <event-statistics.hh>

void EventStatistics::addEvent(long time)
{
  allTimeNumber++;
  events.enqueue(time);
  while(events.head() + frame < time)
    events.dequeue();
  double r = averageRate(time - 1000, time);
  if(r > allTimeMaxRate)
    allTimeMaxRate = r;
}

/// The maximum rate since the first event still in the loop.
double EventStatistics::maxRate()
{
  double rate = 0;
  for(int t = events.head() + 1000; t < events.last(); t += 1000) {
    double r = averageRate(t - 1000, t);
    if(r > rate)
      rate = r;
  }
  return rate;
}

double EventStatistics::averageRate(long currentTime)
{
  if(events.size() < 2)
    return 0;
  return events.size()/(1e-3 * (currentTime- events.head()));
}

double EventStatistics::averageRate(long t1, long t2)
{
  int nb = 0;
  int i = 0;
  if(! events.size()) // No elements yet...
    return 0;
  if(t1 > t2)
    return -1; // Something wrong happened !
  
  while(events[i] < t1) {
    i++;
    if(i >= events.size())
      return 0;
  }

  while(events[i + nb] < t2) {
    nb++;
    if(i + nb >= events.size())
      break;
  }
  return nb/(1e-3 * (t2 - t1));
}

double EventStatistics::movingAverage(long t1, long t2, long dt)
{
  int max = 0;
  int first, last;
  if(dt <= 0)
    return -1;
  last = events.size() - 1;
  while(last >= 0 && events[last] > t2)
    last--;
  if(last < 0)
    return -1;
  first = last;
  while(first >= 0 && events[first] > t1) {
    while(events[last] - events[first] > dt)
      last--;
    if(max < last - first)
      max = last - first;
    first--;
  }
  return max/(dt * 1e-3);
}


QVector<double> EventStatistics::histogram(long t1, long t2, long dt)
{
  int nb = (t2 - t1)/dt + 1;
  long a;
  QVector<double> hist(nb, 0);
  for(int i = 0; i < nb; i++) {
    a = t1 + i * dt;
    if(a + dt > t2)
      a = t2-dt;
    hist[i] = averageRate(a, a+dt);
  }
  return hist;
}
