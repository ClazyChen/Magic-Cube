#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T18:59:38
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagicCube
TEMPLATE = app


SOURCES += main.cpp\
    magiccubeglwidget.cpp \
    singlecube.cpp \
    rotation.cpp

HEADERS  += magiccubeglwidget.h \
    singlecube.h \
    rotation.h \
    mymath.h \
    easy_vector3.h
