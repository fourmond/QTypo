# -*- mode: conf; -*-
# QMake configuration file for QCam

TEMPLATE = app
CONFIG += precompile_header debug
INCLUDEPATH += . src

VERSION = 0.0

PRECOMPILED_HEADER = src/headers.hh

# Input
SOURCES += src/qmain.cc

HEADERS += src/headers.hh
