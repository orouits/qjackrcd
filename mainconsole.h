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
* @file mainconsole.h
* $Author$
* $Date$
* $Revision$
* @brief Header for MainConsole class
*/

#ifndef MAINCONSOLE_H
#define MAINCONSOLE_H

#include <QObject>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QSocketNotifier>
#include "recorder.h"


class MainConsole : public QObject
{
    Q_OBJECT

    QCoreApplication *application;
    Recorder* recorder;
    QTextStream qout;
    QTextStream qin;
    QSocketNotifier qinNotifier;


    QString toGraphText(float level, float fixedLevel);
    QString toBoolText(bool value) {return value ? "true" : "false";}

public:
    explicit MainConsole(Recorder *recorder, QObject *parent = 0);
    ~MainConsole();
signals:
    void quit();

private slots:
    void onRecorderStatusChanged();
    void onInput();

};

#endif // MAINCONSOLE_H
