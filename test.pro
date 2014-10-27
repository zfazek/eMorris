QT       += core gui

TARGET = test_eMorris
TEMPLATE = app
QMAKE_CXXFLAGS += -g -std=c++11 -O2
LIBS +=


SOURCES += src/test.cpp\
    src/table.cpp \
    src/node.cpp \
    src/move.cpp \
    src/mill.cpp \

HEADERS  +=  \
    src/table.h \
    src/node.h \
    src/move.h \
    src/mill.h \

FORMS    +=

