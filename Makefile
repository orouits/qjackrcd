#############################################################################
# Makefile for building: qjackrcd
# Generated by qmake (2.01a) (Qt 4.7.4) on: ven. d�c. 30 19:16:52 2011
# Project:  qjackrcd.pro
# Template: app
# Command: /usr/bin/qmake -config release -o Makefile qjackrcd.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -Ibuild -Ibuild
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/i386-linux-gnu -ljack -lsndfile -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		recorder.cpp \
		qjrmeter.cpp build/moc_mainwindow.cpp \
		build/moc_recorder.cpp \
		build/moc_qjrmeter.cpp \
		build/qrc_qjackrcd.cpp
OBJECTS       = build/main.o \
		build/mainwindow.o \
		build/recorder.o \
		build/qjrmeter.o \
		build/moc_mainwindow.o \
		build/moc_recorder.o \
		build/moc_qjrmeter.o \
		build/qrc_qjackrcd.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		qjackrcd.pro
QMAKE_TARGET  = qjackrcd
DESTDIR       = 
TARGET        = qjackrcd

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): prepare docs locale/qjackrcd_en.qm locale/qjackrcd_fr.qm locale/qjackrcd_it.qm locale/qjackrcd_cs.qm build/ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: qjackrcd.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib/i386-linux-gnu/libQtGui.prl \
		/usr/lib/i386-linux-gnu/libQtCore.prl
	$(QMAKE) -config release -o Makefile qjackrcd.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -config release -o Makefile qjackrcd.pro

dist: 
	@$(CHK_DIR_EXISTS) build/qjackrcd1.0.4 || $(MKDIR) build/qjackrcd1.0.4 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd_en.ts qjackrcd_fr.ts qjackrcd_it.ts qjackrcd_cs.ts build/qjackrcd1.0.4/ && $(COPY_FILE) --parents mainwindow.h recorder.h qjrmeter.h build/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd.qrc build/qjackrcd1.0.4/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp recorder.cpp qjrmeter.cpp build/qjackrcd1.0.4/ && $(COPY_FILE) --parents mainwindow.ui build/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd_en.ts qjackrcd_fr.ts qjackrcd_it.ts qjackrcd_cs.ts build/qjackrcd1.0.4/ && (cd `dirname build/qjackrcd1.0.4` && $(TAR) qjackrcd1.0.4.tar qjackrcd1.0.4 && $(COMPRESS) qjackrcd1.0.4.tar) && $(MOVE) `dirname build/qjackrcd1.0.4`/qjackrcd1.0.4.tar.gz . && $(DEL_FILE) -r build/qjackrcd1.0.4


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) -r docs
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


prepare:
	test -d build || mkdir -p build; test -d locale || mkdir -p locale;

docs: Doxyfile main.cpp mainwindow.cpp recorder.cpp qjrmeter.cpp mainwindow.h recorder.h qjrmeter.h
	doxygen Doxyfile

check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_updateqm_make_all: locale/qjackrcd_en.qm locale/qjackrcd_fr.qm locale/qjackrcd_it.qm locale/qjackrcd_cs.qm
compiler_updateqm_clean:
	-$(DEL_FILE) locale/qjackrcd_en.qm locale/qjackrcd_fr.qm locale/qjackrcd_it.qm locale/qjackrcd_cs.qm
locale/qjackrcd_en.qm: qjackrcd_en.ts
	/usr/bin/lrelease qjackrcd_en.ts -qm locale/qjackrcd_en.qm

locale/qjackrcd_fr.qm: qjackrcd_fr.ts
	/usr/bin/lrelease qjackrcd_fr.ts -qm locale/qjackrcd_fr.qm

locale/qjackrcd_it.qm: qjackrcd_it.ts
	/usr/bin/lrelease qjackrcd_it.ts -qm locale/qjackrcd_it.qm

locale/qjackrcd_cs.qm: qjackrcd_cs.ts
	/usr/bin/lrelease qjackrcd_cs.ts -qm locale/qjackrcd_cs.qm

compiler_moc_header_make_all: build/moc_mainwindow.cpp build/moc_recorder.cpp build/moc_qjrmeter.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/moc_mainwindow.cpp build/moc_recorder.cpp build/moc_qjrmeter.cpp
build/moc_mainwindow.cpp: recorder.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o build/moc_mainwindow.cpp

build/moc_recorder.cpp: recorder.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) recorder.h -o build/moc_recorder.cpp

build/moc_qjrmeter.cpp: qjrmeter.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) qjrmeter.h -o build/moc_qjrmeter.cpp

compiler_rcc_make_all: build/qrc_qjackrcd.cpp
compiler_rcc_clean:
	-$(DEL_FILE) build/qrc_qjackrcd.cpp
build/qrc_qjackrcd.cpp: qjackrcd.qrc \
		qjackrcd.png \
		record-red.png \
		record-orange.png \
		record-green.png
	/usr/bin/rcc -name qjackrcd qjackrcd.qrc -o build/qrc_qjackrcd.cpp

compiler_image_collection_make_all: build/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) build/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: build/ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) build/ui_mainwindow.h
build/ui_mainwindow.h: mainwindow.ui \
		qjrmeter.h
	/usr/bin/uic-qt4 mainwindow.ui -o build/ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_updateqm_clean compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

build/main.o: main.cpp mainwindow.h \
		recorder.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o main.cpp

build/mainwindow.o: mainwindow.cpp mainwindow.h \
		recorder.h \
		build/ui_mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/mainwindow.o mainwindow.cpp

build/recorder.o: recorder.cpp recorder.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/recorder.o recorder.cpp

build/qjrmeter.o: qjrmeter.cpp qjrmeter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qjrmeter.o qjrmeter.cpp

build/moc_mainwindow.o: build/moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_mainwindow.o build/moc_mainwindow.cpp

build/moc_recorder.o: build/moc_recorder.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_recorder.o build/moc_recorder.cpp

build/moc_qjrmeter.o: build/moc_qjrmeter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_qjrmeter.o build/moc_qjrmeter.cpp

build/qrc_qjackrcd.o: build/qrc_qjackrcd.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qrc_qjackrcd.o build/qrc_qjackrcd.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/ 
	-$(INSTALL_PROGRAM) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-$(STRIP) "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(QMAKE_TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/bin/ 


install_translations: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/qjackrcd/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/qjackrcd/ 
	-$(INSTALL_DIR) /home/olivier/src/qjackrcd/locale $(INSTALL_ROOT)/usr/local/share/qjackrcd/


uninstall_translations:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/qjackrcd/locale
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/share/qjackrcd/ 


install_desktop: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/applications/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/applications/ 
	-$(INSTALL_FILE) /home/olivier/src/qjackrcd/qjackrcd.desktop $(INSTALL_ROOT)/usr/local/share/applications/


uninstall_desktop:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/applications/qjackrcd.desktop
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/share/applications/ 


install_icon: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/share/pixmaps/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/share/pixmaps/ 
	-$(INSTALL_FILE) /home/olivier/src/qjackrcd/qjackrcd.png $(INSTALL_ROOT)/usr/local/share/pixmaps/


uninstall_icon:  FORCE
	-$(DEL_FILE) -r $(INSTALL_ROOT)/usr/local/share/pixmaps/qjackrcd.png
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/local/share/pixmaps/ 


install:  install_target install_translations install_desktop install_icon  FORCE

uninstall: uninstall_target uninstall_translations uninstall_desktop uninstall_icon   FORCE

FORCE:

