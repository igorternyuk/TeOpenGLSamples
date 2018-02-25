TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG +=  console
SOURCES += main.c
LIBS += -lglut -lGLU -lGL -lGLEW
QMAKE_CFLAGS += -std=c11
