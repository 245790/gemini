#-------------------------------------------------
#
# Project created by QtCreator 2015-09-05T10:35:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gemini
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        userinterface.cpp \
    grid.cpp \
    gridpainter.cpp \
    propertieswindow.cpp \
    treenode.cpp

HEADERS  += userinterface.h \
    grid.h \
    gridpainter.h \
    treenode.h \
    propertieswindow.h

RESOURCES += \
    treemodel.qrc

DISTFILES +=
