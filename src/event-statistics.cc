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
  events.enqueue(time);
  while(events.head() + frame < time)
    events.dequeue();
}

double EventStatistics::averageRate()
{
  if(events.size() < 2)
    return 0;
  return events.size()/(1e-3 * (events.last() - events.head()));
}

QVector<long> EventStatistics::histogram(int nb)
{
  QVector<long> hist(nb, 0);
  if(events.size() < 1)
    return hist;
  long init = events.last() - frame +1;
  for(int i = 0; i < events.size(); i++) {
    hist[(events[i] - init)*nb/frame] += 1;
  }
  return hist;
}

QPixmap EventStatistics::history()
{
  QVector<long> hist = histogram(frame/1000);
  QPixmap pix(frame/1000*2, 30); // 2 points for one keystroke per second
  QPainter d(&pix);
  QColor c("red");
  QPainterPath p(QPointF(0, 30 - 2*hist[0]));
  d.setPen(c);
  d.eraseRect(pix.rect());
  for(int i = 1; i < hist.size(); i++)
    p.lineTo(i*2, 30 - hist[i]*2);
  d.drawPath(p);

  return pix;
}
