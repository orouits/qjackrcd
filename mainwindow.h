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
* @file mainwindow.h
* $Author$
* $Date$
* $Revision$
* @brief Header for MainWindow class
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QIcon>
#include <QtGui/QMainWindow>
#include "recorder.h"

namespace Ui
{
    class MainWindow;
}

/**
* @class MainWindow
* @brief Encapsulation of all the GUI of the application.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Recorder *recorder, QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void readSettings();
    void writeSettings();

private:
    Ui::MainWindow *ui;
    Recorder *recorder;
    QIcon *iconGreen;
    QIcon *iconRed;
    QIcon *iconOrange;


private slots:
    void on_pauseLevelSpin_valueChanged(double );
    void on_pauseDelaySpin_valueChanged(double );
    void on_recButton_clicked();
    void on_pauseSplitCheck_stateChanged(int );
    void on_postActionCombo_currentIndexChanged(int index);
    void on_postCmdEdit_textChanged(QString );

    void onRecorderStatusChanged();
};

#endif // MAINWINDOW_H
