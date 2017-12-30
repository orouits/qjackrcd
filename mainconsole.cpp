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
* @file mainconsole.cpp
* $Author$
* $Date$
* $Revision$
* @brief Implementation of MainConsole class that manages CLI mode interaction through QT events.
*/

#include "mainconsole.h"
#include <QCoreApplication>

MainConsole::MainConsole(Recorder *recorder, QObject *parent) : QObject(parent), qout(stdout), qin(stdin), qinNotifier(fileno(stdin), QSocketNotifier::Read, this)
{
    // the recorder given by the application.
    this->recorder = recorder;
    application = QCoreApplication::instance();

    qout << application->applicationName() << " " << tr("running in console mode") << endl;
    qout << tr("Congiguration:") << endl;
    qout << "pauseLevel(DB)" << "\t" << recorder->getPauseLevel() << endl;
    qout << "pauseDelay(sec)" << "\t" << recorder->getPauseActivationDelay() << endl;
    qout << "splitMode" << "\t" << toBoolText(recorder->isSplitMode()) << endl;
    qout << "recordAtLaunch" << "\t" << toBoolText(recorder->isRecordAtLaunch()) << endl;
    qout << "connections1" << "\t" << recorder->getJackCns1() << endl;
    qout << "connections2" << "\t" << recorder->getJackCns2() << endl;
    qout << "outputDir" << "\t" << recorder->getOutputDir().absolutePath() << endl;
    qout << "processCmdLine" << "\t" << recorder->getProcessCmdLine() << endl;
    qout << "jackAutoMode" << "\t" << toBoolText(recorder->isJackAutoMode()) << endl;
    qout << "jackTransMode" << "\t" << toBoolText(recorder->isJackTransMode()) << endl;

    connect(recorder, SIGNAL(statusChanged()), this, SLOT(onRecorderStatusChanged()));
    connect(&qinNotifier, SIGNAL(activated(int)), this, SLOT(onInput()));
    connect(this, SIGNAL(quit()), application, SLOT(quit()));
}


MainConsole::~MainConsole()
{

}

void MainConsole::onRecorderStatusChanged()
{
    qout << "\r" << tr("[ENTER] to exit - ");

    if (recorder->isRecording()) {
        if (recorder->isPaused()) {
            qout << tr("Waiting for sound...");
        }
        else {
            qout << tr("Recording...");
        }

    }
    else if (!recorder->isRecordEnabled()) {
        qout << tr("Disabled");
    }
    else {
        qout << tr("Ready");
    }
    qout << " "
         << recorder->getCurrentRecordSize()/1024 << "KB"
         << " [" << toTimeText(recorder->getCurrentRecordTimeSecs()) << "]"
         << " - "
         << recorder->getTotalRecordSize()/1024 << "KB"
         << " [" << toTimeText(recorder->getTotalRecordTimeSecs()) << "]";
    qout << " - " << toGraphText((recorder->getLeftLevel() + recorder->getRightLevel()) / 2, recorder->getPauseLevel());
    qout.flush();
}
void MainConsole::onInput()
{
    qout << tr("Quit") << endl;
    emit quit();
}

QString MainConsole::toGraphText(float level, float fixedLevel)
{
    QString str;
    int min = -40;
    int max = 3;

    for (int i = min; i<=max; i++)
    {
        if (i == (int)fixedLevel) str.append("|");
        else if (i <= level) str.append("#");
        else str.append(".");
    }
    return str;
}

QString MainConsole::toTimeText(long secs)
{
    // cannot use QTime because it wraps after 24h
    return QString("%1:%2:%3").arg(secs/3600,2,10,QLatin1Char('0')).arg((secs/60)%60,2,10,QLatin1Char('0')).arg(secs%60,2,10,QLatin1Char('0'));
}
