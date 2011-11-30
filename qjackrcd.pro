# -------------------------------------------------
# Project created by QtCreator 2010-04-12T22:34:09
# -------------------------------------------------
TARGET = qjackrcd
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    recorder.cpp \
    qjrmeter.cpp
HEADERS += mainwindow.h \
    recorder.h \
    qjrmeter.h
FORMS += mainwindow.ui
RESOURCES += qjackrcd.qrc
LIBS += -ljack -lsndfile
TRANSLATIONS = qjackrcd_en.ts qjackrcd_fr.ts
OTHER_FILES += \
    record-red.png \
    record-orange.png \
    record-green.png \
    README \
    dist.sh
