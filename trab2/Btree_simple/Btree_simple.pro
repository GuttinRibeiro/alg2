TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/btree.cc \
    src/registerparser.cc \
    src/dataparser.cc

HEADERS += \
    src/btree.hh \
    src/list.hh \
    src/registerparser.hh \
    src/dataparser.hh \
    src/commonheader.h
