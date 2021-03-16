#-------------------------------------------------
#
# Project created by QtCreator 2019-07-05T09:54:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WindowsButtons
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

CONFIG += c++11

INCLUDEPATH += interactive_buttons\

SOURCES += \
    interactive_buttons/appendbutton.cpp \
    interactive_buttons/infobutton.cpp \
        main.cpp \
        mainwindow.cpp \
    interactive_buttons/interactivebuttonbase.cpp \
    interactive_buttons/pointmenubutton.cpp \
    interactive_buttons/threedimenbutton.cpp \
    interactive_buttons/waterzoombutton.cpp \
    interactive_buttons/winminbutton.cpp \
    interactive_buttons/winmaxbutton.cpp \
    interactive_buttons/winrestorebutton.cpp \
    interactive_buttons/winclosebutton.cpp \
    interactive_buttons/winmenubutton.cpp \
    interactive_buttons/watercirclebutton.cpp \
    interactive_buttons/waterfloatbutton.cpp

HEADERS += \
    interactive_buttons/appendbutton.h \
    interactive_buttons/infobutton.h \
        mainwindow.h \
    interactive_buttons/interactivebuttonbase.h \
    interactive_buttons/pointmenubutton.h \
    interactive_buttons/threedimenbutton.h \
    interactive_buttons/waterzoombutton.h \
    interactive_buttons/winminbutton.h \
    interactive_buttons/winmaxbutton.h \
    interactive_buttons/winrestorebutton.h \
    interactive_buttons/winclosebutton.h \
    interactive_buttons/winmenubutton.h \
    interactive_buttons/watercirclebutton.h \
    interactive_buttons/waterfloatbutton.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.md
