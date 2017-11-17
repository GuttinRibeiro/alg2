TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cc \
    src/btree.cc \
    src/datahandle.cc \
    src/list.cc \
    src/dataparser.cc \
    src/registerhandle.cc \
    src/registerparser.cc

HEADERS += \
    src/btree.hh \
    src/datahandle.hh \
    src/list.hh \
    src/dataparser.hh \
    src/registerhandle.hh \
    src/registerparser.hh
