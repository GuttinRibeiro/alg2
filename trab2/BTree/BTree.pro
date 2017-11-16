TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cc \
    src/btree.cc \
    src/datahandle.cc

HEADERS += \
    src/btree.hh \
    src/datahandle.hh
