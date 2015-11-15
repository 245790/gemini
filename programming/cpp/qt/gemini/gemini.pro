#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T10:35:12
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gemini
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        userinterface.cpp \
    cell.cpp \
    grid.cpp \
    gridpainter.cpp

HEADERS  += userinterface.h \
    cell.h \
    grid.h \
    gridpainter.h \
    treenode.h
