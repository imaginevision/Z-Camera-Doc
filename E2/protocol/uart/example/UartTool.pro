#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T15:29:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = UartTool
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    msgcodec.cpp \
    cameraconfig.cpp \
    crc16.cpp

HEADERS  += mainwindow.h \
    camera.h \
    msgcodec.h \
    uart_def.h \
    cameraconfig.h \
    imvt_cam_def.h \
    crc16.h

