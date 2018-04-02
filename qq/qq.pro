#-------------------------------------------------
#
# Project created by QtCreator 2018-03-30T10:46:28
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qq
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        qq_widget.cpp \
    qq_drawer.cpp \
    qq_server.cpp \
    qq_client.cpp

HEADERS += \
        qq_widget.h \
    qq_drawer.h \
    qq_server.h \
    qq_client.h

FORMS += \
        qq_widget.ui \
    qq_server.ui \
    qq_client.ui

RESOURCES += \
    images.qrc
