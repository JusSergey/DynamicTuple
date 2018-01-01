TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    DynamicTuple.cpp \
    test.cpp

HEADERS += \
    DynamicTuple.h \
    test.h
