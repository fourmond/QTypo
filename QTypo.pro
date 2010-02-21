# -*- mode: conf; -*-
# QMake configuration file for QCam

TEMPLATE = app
CONFIG += precompile_header debug
INCLUDEPATH += . src

VERSION = 0.0

PRECOMPILED_HEADER = src/headers.hh

LIBS += -lXtst


# Input
SOURCES += src/qmain.cc \
	src/xrecord-gather.cc src/event-statistics.cc \
	src/mainwin.cc

HEADERS += src/headers.hh src/xrecord-gather.hh \
	src/event-statistics.hh src/mainwin.hh
