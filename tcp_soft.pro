#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T22:51:23
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcp_soft
TEMPLATE = app


SOURCES += main.cpp\
    DirectoryTableModel.cpp \
    MainWindow.cpp \
    DirectoryRecord.cpp \
    DirectoryConnect.cpp \
    DirectorySyncHelper.cpp

HEADERS  += \
    DirectoryTableModel.h \
    MainWindow.h \
    DirectoryRecord.h \
    DirectoryConnect.h \
    DirectorySyncHelper.h

FORMS    += mainwindow.ui

RESOURCES += \
    res/res.qrc
