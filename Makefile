#############################################################################
# Makefile for building: qjackrcd
# Generated by qmake (2.01a) (Qt 4.7.4) on: jeu. d�c. 29 13:34:25 2011
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
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
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

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		recorder.cpp \
		qjrmeter.cpp moc_mainwindow.cpp \
		moc_recorder.cpp \
		moc_qjrmeter.cpp \
		qrc_qjackrcd.cpp
OBJECTS       = main.o \
		mainwindow.o \
		recorder.o \
		qjrmeter.o \
		moc_mainwindow.o \
		moc_recorder.o \
		moc_qjrmeter.o \
		qrc_qjackrcd.o
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
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
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

$(TARGET): locale/qjackrcd_en.qm locale/qjackrcd_fr.qm locale/qjackrcd_it.qm locale/qjackrcd_cs.qm ui_mainwindow.h $(OBJECTS)  
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
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
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
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/i386-linux-gnu/libQtGui.prl:
/usr/lib/i386-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -config release -o Makefile qjackrcd.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/qjackrcd1.0.4 || $(MKDIR) .tmp/qjackrcd1.0.4 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd_en.ts qjackrcd_fr.ts qjackrcd_it.ts qjackrcd_cs.ts .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents mainwindow.h recorder.h qjrmeter.h .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd.qrc .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp recorder.cpp qjrmeter.cpp .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents mainwindow.ui .tmp/qjackrcd1.0.4/ && $(COPY_FILE) --parents qjackrcd_en.ts qjackrcd_fr.ts qjackrcd_it.ts qjackrcd_cs.ts .tmp/qjackrcd1.0.4/ && (cd `dirname .tmp/qjackrcd1.0.4` && $(TAR) qjackrcd1.0.4.tar qjackrcd1.0.4 && $(COMPRESS) qjackrcd1.0.4.tar) && $(MOVE) `dirname .tmp/qjackrcd1.0.4`/qjackrcd1.0.4.tar.gz . && $(DEL_FILE) -r .tmp/qjackrcd1.0.4


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) -r docs
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


docs: main.cpp mainwindow.cpp recorder.cpp qjrmeter.cpp mainwindow.h recorder.h qjrmeter.h
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

compiler_moc_header_make_all: moc_mainwindow.cpp moc_recorder.cpp moc_qjrmeter.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_recorder.cpp moc_qjrmeter.cpp
moc_mainwindow.cpp: recorder.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_recorder.cpp: recorder.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) recorder.h -o moc_recorder.cpp

moc_qjrmeter.cpp: qjrmeter.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) qjrmeter.h -o moc_qjrmeter.cpp

compiler_rcc_make_all: qrc_qjackrcd.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_qjackrcd.cpp
qrc_qjackrcd.cpp: qjackrcd.qrc \
		qjackrcd.png \
		record-red.png \
		record-orange.png \
		record-green.png
	/usr/bin/rcc -name qjackrcd qjackrcd.qrc -o qrc_qjackrcd.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
ui_mainwindow.h: mainwindow.ui \
		qjrmeter.h
	/usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_updateqm_clean compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		recorder.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		recorder.h \
		ui_mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

recorder.o: recorder.cpp recorder.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o recorder.o recorder.cpp

qjrmeter.o: qjrmeter.cpp qjrmeter.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qjrmeter.o qjrmeter.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_recorder.o: moc_recorder.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_recorder.o moc_recorder.cpp

moc_qjrmeter.o: moc_qjrmeter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_qjrmeter.o moc_qjrmeter.cpp

qrc_qjackrcd.o: qrc_qjackrcd.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_qjackrcd.o qrc_qjackrcd.cpp

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

