TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/btree.cc \
    src/registerparser.cc \
    src/datahandle.cc \
    src/loghandle.cc

HEADERS += \
    src/btree.hh \
    src/list.hh \
    src/registerparser.hh \
    src/sharedheader.h \
    src/datahandle.hh \
    src/loghandle.hh
