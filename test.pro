QT       += core gui

TARGET = test_eMorris
TEMPLATE = app
QMAKE_CXXFLAGS += -g -std=c++11


SOURCES += src/test.cpp\
    src/table.cpp \
    src/move.cpp \
    src/mill.cpp \

HEADERS  +=  \
    src/table.h \
    src/mill.h \

FORMS    +=

