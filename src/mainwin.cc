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

MainWin::MainWin(XRecordGather *g) : gatherer(g)
{
  QWidget * central = new QWidget(this);
  QVBoxLayout * layout = new QVBoxLayout(central);
  imgDisplay = new QLabel();
  layout->addWidget(imgDisplay);
  textDisplay = new QLabel();
  layout->addWidget(textDisplay);
  setCentralWidget(central);
  connect(gatherer, SIGNAL(newEvents()), SLOT(updateDisplay()));
  updateDisplay();
}

void MainWin::updateDisplay()
{
  textDisplay->setText(tr("Avg: %1 k/s Max: 0 k/s").
		       arg(gatherer->averageRate()));
  imgDisplay->setPixmap(gatherer->history());
}
