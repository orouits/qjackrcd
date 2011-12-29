/***************************************************************************
 Copyright (C) 2011 - Olivier ROUITS <olivier.rouits@free.fr>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ***************************************************************************/
/**
* @file main.cpp
* $Author$
* $Date$
* $Revision$
*
* QJackRcd is a simple QT application to record JACK server outputs (use it with QJackCtl)
*
* The initial goal of this project is to record from an old tape, with an automatic split/pause feature when you are away. Another goal is to have a litle tool ready to use with a minimum of configuration for simple stereo recording needs.
* It manages natively silence by threshold and activation time. Silence event can be used to pause the record or to split files by closing the current record and opening a new one.
* Optionaly QJackRcd is enable to post-process each file record at closure in background mode. the command is a simple bash command.
*
* Made with QTCreator
* It Depends on jack, sndfile and qt4 libraries.
*/

/**
* @mainpage
*
* QJackRcd is a simple QT application to record JACK server outputs (use it with QJackCtl)
*
* The initial goal of this project is to record from an old tape, with an automatic split/pause feature when you are away. Another goal is to have a litle tool ready to use with a minimum of configuration for simple stereo recording needs.
* It manages natively silence by threshold and activation time. Silence event can be used to pause the record or to split files by closing the current record and opening a new one.
* Optionaly QJackRcd is enable to post-process each file record at closure in background mode. the command is a simple bash command.
*
* Made with QTCreator
* It Depends on jack, sndfile and qt4 libraries.
*/

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"

#define REC_JK_NAME "QJackRcd" ///< Name of the recorder instance used in jack client name

/**
 * @fn int main (int argc, char *argv[])
 * @brief Program enrty.
 *
 * Main doesn't take any special paramerter, only standard QT parameters.
 *
 * @return 0 if normal GUI quit.
 */
int main(int argc, char *argv[])
{
    // The application and translator
    QApplication application(argc, argv);
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (!translator.load(QString("qjackrcd_") + locale, "/usr/share/qjackrcd/locale"))
        translator.load(QString("qjackrcd_") + locale, "locale"); // for tests inplace
    application.installTranslator(&translator);

    // The recorder
    Recorder recorder(REC_JK_NAME);

    // The window
    MainWindow window(&recorder);

    // Go !
    window.show();
    return application.exec();
}
