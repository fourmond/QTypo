/** 
    \file mainwin.hh Main window for QTypo
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

#ifndef __MAINWIN_HH
#define __MAINWIN_HH

#include <xrecord-gather.hh>

/// Main window of QTypo.
class MainWin : public QMainWindow {

  Q_OBJECT;

  XRecordGather * gatherer;

  /// The whole display
  QLabel * display;


  /// The timer responsible for the update.
  QTimer refreshTimer;

  /// The width of the display, in pixels
  int displayWidth();

  /// The size in terms of the display
  QSize displaySize();

  /// The size of the text band down.
  int textSize;

  /// The total size of the inner widget.
  QSize totalSize();

  /// The left margin (X axis ?)
  int leftMargin;

  /// The right margin (X axis ?)
  int rightMargin;

  /// The last XRecordGather::currentTime() when the display was
  /// updated, or -1
  long lastTime;

  /// The font used for display
  QFont font;

  /// The height of the display.
  int displayHeight;

  int totalTime();

  /// The structure of the display. Arrays of pairs:
  /// \li the first element is the time in second for the average
  /// \li the second is the number of averages
  ///
  /// from right to left, ie from most recent to most ancient.
  QList< QPair<int,int> > displayScales;

  /// Returns the position of the pixels to be drawn onto the
  /// display. Need to multiply absissa by 2 to actually get the pixels.
  QVector<double> ratesData(long time);
  

public:
  MainWin(XRecordGather *g);
  ~MainWin() {;};

public slots:
  /// Updates the display to reflect recent changes in the statistics
  void updateDisplay();
  
};

#endif
