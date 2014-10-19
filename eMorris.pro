#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T13:40:19
#
#-------------------------------------------------

QT       += core gui

TARGET = eMorris
TEMPLATE = app
QMAKE_CXXFLAGS += -g -std=c++11
LIBS += -pthread


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/centralwidget.cpp \
    src/table.cpp \
    src/mill.cpp \
    src/move.cpp \
    src/canvas.cpp

HEADERS  += src/mainwindow.h \
    src/centralwidget.h \
    src/table.h \
    src/mill.h \
    src/move.h \
    src/canvas.h

FORMS    +=
