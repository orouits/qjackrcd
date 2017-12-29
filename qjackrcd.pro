# -------------------------------------------------
# Project created by QtCreator 2010-04-12T22:34:09
# -------------------------------------------------
isEmpty(PREFIX):PREFIX = /usr/local
isEmpty(BINDIR):BINDIR = $$PREFIX/bin
isEmpty(DATADIR):DATADIR = $$PREFIX/share

VERSION = 1.2.0
TARGET = qjackrcd
TEMPLATE = app

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
LOCALE_DIR = locale

QT += core gui widgets

SOURCES += main.cpp \
    mainwindow.cpp \
    recorder.cpp \
    qjrmeter.cpp \
    mainconsole.cpp

HEADERS += \
    mainwindow.h \
    recorder.h \
    qjrmeter.h \
    mainconsole.h

FORMS += mainwindow.ui

RESOURCES += qjackrcd.qrc

LIBS += -ljack -lsndfile

TRANSLATIONS += \
    qjackrcd_en.ts \
    qjackrcd_fr.ts \
    qjackrcd_it.ts \
    qjackrcd_cs.ts \
    qjackrcd_de.ts \

DISTFILES += \
    Doxyfile \
    qjackrcd.1.sgml

OTHER_FILES += \
    README.md \
    LICENSE \
    publish.sh

# prepare target
prepare.target = prepare
prepare.commands =  test -d $$UI_DIR || mkdir -p $$UI_DIR; \
    test -d $$LOCALE_DIR || mkdir -p $$LOCALE_DIR;
prepare.depends = 
QMAKE_EXTRA_TARGETS += prepare
PRE_TARGETDEPS += prepare

# custom translation compiler
updateqm.input = $$TRANSLATIONS
updateqm.output = $$LOCALE_DIR/${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease ${QMAKE_FILE_IN} -qm $$LOCALE_DIR/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

# custom inslall files
target.path = $$BINDIR
translations.path = $$DATADIR/qjackrcd
translations.files = $$LOCALE_DIR
desktop.path = $$DATADIR/applications
desktop.files = qjackrcd.desktop
icon.path = $$DATADIR/pixmaps
icon.files = qjackrcd.png
manpage.path = $$DATADIR/man/man1
manpage.files = qjackrcd.1.gz
INSTALLS += target \
            translations \
            desktop \
            icon \
            manpage \

# custom docs target
doxy.target = doxy
doxy.commands = doxygen Doxyfile
doxy.depends = Doxyfile $$SOURCES $$HEADERS
QMAKE_EXTRA_TARGETS += doxy
QMAKE_CLEAN += -r docs/html

# man pages
man.target = qjackrcd.1.gz
man.depends = qjackrcd.1.sgml
man.commands = docbook-to-man $$man.depends | gzip > $$man.target
QMAKE_EXTRA_TARGETS += man
POST_TARGETDEPS += $$man.target
QMAKE_CLEAN += $$man.target


