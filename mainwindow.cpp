// $Id$
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
// $LastChangedBy$
// $LastChangedRevision$
// $LastChangeDate$
// $HeadURL$

#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    timer = new QTimer(this);
    iconGreen = new QIcon(":/qjackrcd/record-green.png");
    iconRed = new QIcon(":/qjackrcd/record-red.png");
    iconOrange= new QIcon(":/qjackrcd/record-orange.png");

    ui->setupUi(this);

    ui->vuMeter->setColorBack(palette().window().color());

    ui->postActionCombo->addItem("OGG","sox ${0} ${0%%wav}ogg");
    ui->postActionCombo->addItem("MP3","sox ${0} ${0%%wav}mp3");

    recorder = new Recorder();
    pprocessor = new PProcessor();

    ui->pauseLevelSpin->setValue(recorder->getPauseLevel());
    ui->pauseSplitCheck->setChecked(recorder->isSplitMode());
    ui->vuMeter->setCompLevel(recorder->getPauseLevel());
    ui->statusBar->showMessage(tr("Ready"));

    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));

    timer->setInterval(REFRESHOKMS);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete timer;
    if (recorder) delete recorder;
    if (pprocessor) delete pprocessor;
    delete iconGreen;
    delete iconRed;
    delete iconOrange;
    delete ui;
}


void MainWindow::on_recButton_clicked()
{    
    if (!recorder) return;
    if (recorder->getStatus() > RECOFF) {
        recorder->stop();
    }
    else {
        recorder->start();
    }
}

void MainWindow::on_pauseDelaySpin_valueChanged(double secs)
{
    if (!recorder) return;
    recorder->setPauseActivationDelay(secs);
}

void MainWindow::on_pauseLevelSpin_valueChanged(double level)
{
    if (!recorder) return;
    recorder->setPauseLevel(level);
    ui->vuMeter->setCompLevel(level);
}

void MainWindow::on_timer()
{
    if (!recorder) return;
    if (recorder->getStatus() < RECOFF) {
        close();
    }
    ui->vuMeter->setLeftLevel(recorder->getLeftLevel());
    ui->vuMeter->setRightLevel(recorder->getRightLevel());
    ui->recDiskProgress->setValue(recorder->getDiskSpace());
    if (ui->recFileEdit->text() != recorder->getFilePath()) {
        ui->recFileEdit->setText(recorder->getFilePath());
        pprocessor->launchFileProcess();
        QString str = recorder->getFilePath();
        pprocessor->setFileName(str);
    }
    if (ui->postLastEdit->text() != pprocessor->getLaunchedFileName())
        ui->postLastEdit->setText(pprocessor->getLaunchedFileName());

    if (recorder->getStatus() > RECOFF) {
        if (recorder->getStatus() == RECWAIT) {
            ui->recButton->setIcon(*iconOrange);
            ui->statusBar->showMessage(tr("Waiting for sound..."));
        }
        else {
            ui->recButton->setIcon(*iconRed);
            ui->statusBar->showMessage(tr("Recording..."));
        }
    }
    else {
        ui->recButton->setIcon(*iconGreen);
        ui->statusBar->showMessage(tr("Ready"));
    }
}

void MainWindow::on_pauseSplitCheck_stateChanged(int value)
{
    if (!recorder) return;
    recorder->setSplitMode(value != 0);
}

void MainWindow::on_postActionCombo_currentIndexChanged(int index)
{
    ui->postCmdEdit->setText(ui->postActionCombo->itemData(index).toString());
}

void MainWindow::on_postCmdEdit_textChanged(QString text)
{
    pprocessor->setCmdLine(text);
}
