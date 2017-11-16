TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    btree.cc \
    filetemplate.cc \
    filestruct.cc

HEADERS += \
    btree.hh \
    filetemplate.hh \
    filestruct.hh
