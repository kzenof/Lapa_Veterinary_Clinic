QT += core gui widgets
CONFIG += c++17
TEMPLATE = app
TARGET = VetClinic

SOURCES += \
    main.cpp \
    startwindow.cpp \
    loginwindow.cpp \
    registrarwindow.cpp \
    adminwindow.cpp \
    clientwindow.cpp

HEADERS += \
    startwindow.h \
    loginwindow.h \
    registrarwindow.h \
    adminwindow.h \
    clientwindow.h \
    userdata.h
