# -------------------------------------------------
# Project created by QtCreator 2010-04-12T22:34:09
# -------------------------------------------------
isEmpty(PREFIX):PREFIX = /usr/local
isEmpty(QMAKE_LRELEASE):QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
isEmpty(BINDIR):BINDIR = $$PREFIX/bin
isEmpty(DATADIR):DATADIR = $$PREFIX/share

VERSION = 1.0.4
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
TRANSLATIONS += qjackrcd_en.ts \
    qjackrcd_fr.ts \
    qjackrcd_it.ts \
    qjackrcd_cs.ts
OTHER_FILES += \
    README \
    dist.sh \
    qjackrcd.desktop

updateqm.input = TRANSLATIONS
updateqm.output = locale/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm locale/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

target.path = $$BINDIR

translations.path = $$DATADIR/qjackrcd
translations.files = locale

desktop.path = $$DATADIR/applications
desktop.files += qjackrcd.desktop

icon.path = $$DATADIR/pixmaps
icon.files = qjackrcd.png

INSTALLS += target \
            translations \
            desktop \
            icon
