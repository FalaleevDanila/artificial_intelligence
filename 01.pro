#-------------------------------------------------
#
# Project created by QtCreator 2019-10-27T03:43:09
#
#-------------------------------------------------

QT       += core gui
QT       += testlib
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 01
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += include

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        sources/bullet.cpp \
        sources/main.cpp \
        sources/mainwindow.cpp \
        sources/mousecontrol.cpp \
        sources/npc.cpp \
        sources/player.cpp \
        sources/test_class.cpp \
        sources/widget.cpp

HEADERS += \
        include/bullet.h \
        include/collising.h \
        include/mainwindow.h \
        include/man.h \
        include/mousecontrol.h \
        include/npc.h \
        include/player.h \
        include/target.h \
        include/test_class.h \
        include/wall.h \
        include/way.h \
        include/widget.h

FORMS += \
        sources/mainwindow.ui \
        sources/widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
