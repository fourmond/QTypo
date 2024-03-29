/** 
    qmain.cc: entry point of QTypo
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
#include <xrecord-gather.hh>
#include <mainwin.hh>

#include <stdio.h>

int main(int argc, char ** argv)
{
  QApplication main(argc, argv);
  main.setApplicationName("QTypo");

  XRecordGather gatherer;
  gatherer.startGathering();

  MainWin win(&gatherer);

  win.show();
  main.exec();
}
