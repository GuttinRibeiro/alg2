TEMPLATE = app
CONFIG -= console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/btree.cc \
    src/registerparser.cc \
    src/datahandle.cc \
    src/loghandle.cc \
    main.cc

HEADERS += \
    src/btree.hh \
    src/list.hh \
    src/registerparser.hh \
    src/datahandle.hh \
    src/loghandle.hh \
    src/sharedheader.hh
