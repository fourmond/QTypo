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

MainWin::MainWin(XRecordGather *g) : gatherer(g), lastTime(-1)
{
  display = new QLabel();
  setCentralWidget(display);

  refreshTimer.setSingleShot(false);
  refreshTimer.start(4000);

  displaySize = QSize(200,40);
  textSize = 15;
  leftMargin = 15;
  rightMargin = 5;
  totalSize = QSize(displaySize.width() + leftMargin + rightMargin, 
		    displaySize.height() + textSize);
  
  /// @todo customize !
  rateDisplay = QPixmap(displaySize);
  rateDisplay.fill(); /// \todo customize background color...

  connect(&refreshTimer, SIGNAL(timeout()), SLOT(updateDisplay()));
  updateDisplay();

  resize(totalSize);
  setWindowFlags(Qt::FramelessWindowHint);
}

void MainWin::updateDisplay()
{
  long currentTime = gatherer->currentTime();
  if(currentTime <= lastTime)
    return;
  QPixmap area = QPixmap(totalSize);
  QPainter p(&area);
  p.eraseRect(area.rect());

  QFont font;
  font.setPointSize(8);
  p.setFont(font);

  /// @todo make it all neat and clean, using the right functions...
  p.drawText(QRect(0,displaySize.height(), totalSize.width(),
		   totalSize.height() - displaySize.height()),
	     Qt::AlignCenter, 
	     tr("A: %1 k/s M: %2 k/s T: %4 k AM: %3 k/s").
	     arg(gatherer->events()->averageRate(currentTime),0,'f', 1).
	     arg(gatherer->events()->maxRate(),0,'f', 1).
	     arg(gatherer->events()->allTimeMaxRate,0,'f', 1).
	     arg(gatherer->events()->allTimeNumber));


  if(lastTime >= 0) {
    QPixmap nd(displaySize);
    nd.fill();
    {
      QPainter np(&nd);
      double avg = gatherer->events()->averageRate(lastTime, currentTime);
      // shift the old pixmap
      np.drawPixmap(0,0,rateDisplay, 2,0, -1, -1);
      
      // Now, draw some bar:
      np.fillRect(displaySize.width() - 2, 
		  displaySize.height() - 1, 2, - avg * 2, 
		  Qt::SolidPattern);
    }
    rateDisplay = nd;
  }

  // Last draw the rateDisplay
  p.drawPixmap(leftMargin,0, rateDisplay);
  QPen pen;  

  pen.setWidth(1);
  pen.setBrush(Qt::blue);
  pen.setStyle(Qt::DashLine);
  // That's not reallt clean, but, well...
  p.setPen(pen);
  p.drawLine(leftMargin, displaySize.height(), 
	     totalSize.width(), displaySize.height());
  p.drawLine(leftMargin, displaySize.height() - 20, 
	     totalSize.width(), displaySize.height() - 20);
  pen.setStyle(Qt::DotLine);
  p.setPen(pen);
  p.drawLine(leftMargin, displaySize.height() - 10, 
	     totalSize.width(), displaySize.height() - 10);
  p.drawLine(leftMargin, displaySize.height() - 30, 
	     totalSize.width(), displaySize.height() - 30);

  // Legends ?


  display->setPixmap(area);
  lastTime = currentTime;
}
