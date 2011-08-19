# -------------------------------------------------
# Project created by QtCreator 2010-04-12T22:34:09
# -------------------------------------------------
TARGET = qjackrcd
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    recorder.cpp \
    qjrmeter.cpp \
    pprocessor.cpp
HEADERS += mainwindow.h \
    recorder.h \
    qjrmeter.h \
    pprocessor.h
FORMS += mainwindow.ui
RESOURCES += qjackrcd.qrc
LIBS += -ljack -lsndfile
