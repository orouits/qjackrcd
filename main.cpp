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
* @brief Main function implementation
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
#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QCommandLineParser>

#define RCD_APP_NAME "qjackrcd" // Name of the recorder instance used in jack client name
#define RCD_APP_VERSION "1.1" // Version of recorder application
#define RCD_JK_NAME "QJackRcd" // Name of the recorder instance used in jack client name

/**
 * @fn int main (int argc, char *argv[])
 * @brief Program entry.
 *
 * Main doesn't take any special paramerter, only standard QT parameters.
 *
 * @return 0 if normal GUI quit.
 */

//=============================================================================
// Settings methods
//=============================================================================

void readSettings(Recorder &recorder, QSettings &settings, QCommandLineParser &parser)
{
    if (!parser.isSet("no-settings")) {
        settings.beginGroup("Recorder");
        recorder.setPauseLevel(settings.value("pauseLevel", -20).toFloat());
        recorder.setPauseActivationDelay(settings.value("pauseActivationDelay", 3).toInt());
        recorder.setSplitMode(settings.value("splitMode", false).toBool());
        recorder.setProcessCmdLine(settings.value("processCmdLine", "").toString());
        recorder.setJackConnections1(settings.value("connections1", "").toString());
        recorder.setJackConnections2(settings.value("connections2", "").toString());
        recorder.setJackAutoMode(settings.value("jackAuto", true).toBool());
        recorder.setJackTransMode(settings.value("jackTrans", true).toBool());
        recorder.setOutputDir(settings.value("outputDir", QDir::home().absolutePath()).toString());
        recorder.setRecordAtLaunch(settings.value("recordAtLaunch", false).toBool());
        settings.endGroup();
    }

    if (parser.isSet("l")) recorder.setPauseLevel(parser.value("pauseLevel").toInt());
    if (parser.isSet("d")) recorder.setPauseActivationDelay(parser.value("pauseDelay").toInt());
    if (parser.isSet("s")) recorder.setSplitMode(true);
}

void writeSettings(Recorder &recorder, QSettings &settings, QCommandLineParser &parser)
{
    if (!parser.isSet("no-settings")) {
        settings.beginGroup("Recorder");
        settings.setValue("pauseLevel", recorder.getPauseLevel());
        settings.setValue("pauseActivationDelay", recorder.getPauseActivationDelay());
        settings.setValue("splitMode", recorder.isSplitMode());
        settings.setValue("processCmdLine", recorder.getProcessCmdLine());
        settings.setValue("connections1", recorder.getJackConnections1());
        settings.setValue("connections2", recorder.getJackConnections2());
        settings.setValue("jackAuto", recorder.isJackAutoMode());
        settings.setValue("jackTrans", recorder.isJackTransMode());
        settings.setValue("outputDir", recorder.getOutputDir().absolutePath());
        settings.setValue("recordAtLaunch", recorder.isRecordAtLaunch());
        settings.endGroup();
    }
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char *argv[])
{
    int exitcode;

    // Application
    QApplication application(argc, argv);

    application.setApplicationName(RCD_APP_NAME);
    application.setApplicationVersion(RCD_APP_VERSION);

    // Translator
    QString locale = QLocale::system().name();
    QTranslator translator;

    // for packaged system install
    if (!translator.load(QString("qjackrcd_") + locale, "/usr/share/qjackrcd/locale"))
        // for install from source
        if (!translator.load(QString("qjackrcd_") + locale, "/usr/local/share/qjackrcd/locale"))
            // for dev test
            translator.load(QString("qjackrcd_") + locale, "locale");

    application.installTranslator(&translator);

    // CLI && Settings
    QCommandLineParser parser;
    parser.setApplicationDescription("Jack simple stereo recorder");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption("no-gui", "No GUI mode."));
    parser.addOption(QCommandLineOption("no-settings", "Do not store settings"));

    parser.addOption(QCommandLineOption("l", "Pause level in db.", "pauseLevel"));
    parser.addOption(QCommandLineOption("d", "Pause activation delay in seconds.", "pauseDelay"));
    parser.addOption(QCommandLineOption("s", "Split files mode."));

    parser.process(application);

    QSettings settings(application.applicationName(), application.applicationName());

    // Recorder
    Recorder recorder(RCD_JK_NAME);
    readSettings(recorder, settings, parser);
    recorder.start();

    if (!parser.isSet("no-gui")) {
        // Window
        MainWindow window(&recorder);
        // Go !
        window.show();
        exitcode =  application.exec();
    }
    else {
        qInfo("%s %s...", application.applicationName().toLatin1(), application.translate("","Running").toLatin1());
        exitcode =  application.exec();
    }


    writeSettings(recorder, settings, parser);

    return exitcode;
}
