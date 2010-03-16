/*
    mainwin.cc: Main window for QTypo
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
#include <mainwin.hh>

#include <stdio.h>

int MainWin::displayWidth()
{
  int i = 0;
  for(int j = 0; j < displayScales.size(); j++)
    i += displayScales[j].second;
  return i*2;
}

QSize MainWin::displaySize()
{
  return QSize(displayWidth(), displayHeight);
}


QSize MainWin::totalSize()
{
  return QSize(displayWidth() + leftMargin + rightMargin, 
	       displayHeight + textSize);
}

int MainWin::totalTime()
{
  int i = 0;
  for(int j = 0; j < displayScales.size(); j++)
    i += displayScales[j].first * displayScales[j].second;
  return i;
}

MainWin::MainWin(XRecordGather *g) : gatherer(g), lastTime(-1)
{
  display = new QLabel();
  setCentralWidget(display);

  refreshTimer.setSingleShot(false);
  refreshTimer.start(1000);

  textSize = 15;
  leftMargin = 15;
  rightMargin = 5;

  displayHeight = 30;

  displayScales << QPair<int,int>(1,20)
		<< QPair<int,int>(5,30)
		<< QPair<int,int>(30,50);

  // set the frame correctly.
  g->events()->frame = totalTime() * 1000;

  
  connect(&refreshTimer, SIGNAL(timeout()), SLOT(updateDisplay()));
  updateDisplay();

  resize(totalSize());
  setWindowFlags(Qt::FramelessWindowHint);

  font.setPixelSize(9);
}

QVector<double> MainWin::ratesData(long time)
{
  QVector<double> values;
  long t = time;
  for(int i = 0; i < displayScales.size(); i++) {
    int dt = displayScales[i].first;
    int nb = displayScales[i].second;
    t = t - dt * 1000 * nb;
    values = gatherer->events()->
      histogram(t, t + dt * 1000 * nb, dt * 1000) + values;
  }
  return values;
}

void MainWin::updateDisplay()
{
  long currentTime = gatherer->currentTime();
  if(currentTime <= lastTime)
    return;
  QPixmap area = QPixmap(totalSize());
  QPainter p(&area);
  p.eraseRect(area.rect());

  p.setFont(font);

  /// @todo make it all neat and clean, using the right functions...
  p.drawText(QRect(0,displayHeight, totalSize().width(),
		   totalSize().height() - displayHeight),
	     Qt::AlignCenter, 
	     tr("Rates: %1 %2 %3 Max: %4 k/s T: %5 k").
	     arg(gatherer->events()->movingAverage(currentTime,1000),0,'f', 1).
	     arg(gatherer->events()->movingAverage(currentTime,5000),0,'f', 1).
	     arg(gatherer->events()->movingAverage(currentTime,30000),0,'f', 1).
	     arg(gatherer->events()->allTimeMaxRate,0,'f', 1).
	     arg(gatherer->events()->allTimeNumber));

  // Set the background
  QPen pen;  



  if(lastTime >= 0) {
    /// \todo using a subpixmap isn't that great. I probably should
    /// try using a way to select the viewport. Later.
    QPixmap nd(displaySize());
    nd.fill(Qt::transparent);
    QPainter np(&nd);
    QColor color("darkgreen");


    // Drawing horizontal background lines.
    pen.setWidth(1);
    pen.setBrush(Qt::blue);

    for(int i = 0; i < displayHeight; i += 10) {
      if(i/10 % 2)
	pen.setStyle(Qt::DotLine);
      else
	pen.setStyle(Qt::DashLine);
      np.setPen(pen);
      
      np.drawLine(0, displayHeight - i, 
		  displaySize().width(), displayHeight - i);
    }

    // Drawing vertical lines corresponding to the scale transitions
    pen.setStyle(Qt::DashLine);
    np.setPen(pen);
    int j = 0;
    for(int i = displayScales.size() - 1; i >= 0 ; i--) {
      j += displayScales[i].second * 2;
      np.drawLine(j, 0, j, displayHeight);
    }

    // Now drawing the actual data
    QVector<double> values = ratesData(currentTime);
    QPainterPath path;
    path.moveTo(0,displayHeight - values[0] * 2);
    for(int i = 1; i < values.size(); i++) {
      path.lineTo(i * 2, displayHeight - values[i] * 2);
    }
    np.strokePath(path, QPen(color));
    np.setOpacity(0.5);
    // We now close the path.
    path.lineTo((values.size() - 1) * 2, displayHeight);
    path.lineTo(0,displayHeight);
    np.fillPath(path, QBrush(color));


    p.drawPixmap(leftMargin,0, nd);    
  }

  // Draw a border ?
  pen.setWidth(1);
  pen.setBrush(Qt::black);
  pen.setStyle(Qt::SolidLine);
  p.setPen(pen);
  
  p.drawRect(QRect(QPoint(leftMargin, 0), displaySize()));


  display->setPixmap(area);
  lastTime = currentTime;
}
