TEMPLATE = app
TARGET = Spectro

QT += core gui widgets

DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qwt

LIBS = -lusb -lqwt

HEADERS += renderarea.h spectro.h timestamp.h \
    predictions.h
SOURCES += renderarea.cpp spectro.cpp timestamp.cpp \
    predictions.cpp
