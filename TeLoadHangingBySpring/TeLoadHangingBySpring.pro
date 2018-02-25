TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lglut -lGLU -lGL -lGLEW
QMAKE_CFLAGS += -std=c11
SOURCES += main.c
