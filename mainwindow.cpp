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
* @file mainwindow.cpp
* $Author$
* $Date$
* $Revision$
* @brief Implementation of MainWindow class
*/

#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>

#define POSTCMD_NONE_ITEM 0
#define POSTCMD_OGG_ITEM 1
#define POSTCMD_MP3_ITEM 2
#define POSTCMD_CUSTOM_ITEM 3

//=============================================================================
// Constructor / destructor
//=============================================================================
MainWindow::MainWindow(Recorder *recorder, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // the recorder given by the application.
    this->recorder = recorder;

    iconGreen = new QIcon(":/qjackrcd/record-green.png");
    iconRed = new QIcon(":/qjackrcd/record-red.png");
    iconOrange= new QIcon(":/qjackrcd/record-orange.png");

    ui->setupUi(this);

    ui->vuMeter->setColorBack(palette().window().color());
    ui->postActionCombo->setItemData(POSTCMD_NONE_ITEM,"");
    ui->postActionCombo->setItemData(POSTCMD_OGG_ITEM,"sox ${0} ${0%%wav}ogg");
    ui->postActionCombo->setItemData(POSTCMD_MP3_ITEM,"sox ${0} ${0%%wav}mp3");

    ui->pauseLevelSpin->setValue(recorder->getPauseLevel());
    ui->pauseDelaySpin->setValue(recorder->getPauseActivationDelay());
    ui->pauseSplitCheck->setChecked(recorder->isSplitMode());
    ui->vuMeter->setCompLevel(recorder->getPauseLevel()); 
    ui->statusBar->showMessage(tr("Ready"));
    ui->postCmdEdit->setText(recorder->getProcessCmdLine());

    int postItem = ui->postActionCombo->findData(recorder->getProcessCmdLine());
    if (postItem >= 0)
        ui->postActionCombo->setCurrentIndex(postItem);
    else {
        ui->postActionCombo->setItemData(POSTCMD_CUSTOM_ITEM, recorder->getProcessCmdLine());
        ui->postActionCombo->setCurrentIndex(POSTCMD_CUSTOM_ITEM);
    }

    ui->optOutputDirEdit->setText(recorder->getOutputDir().absolutePath());
    ui->optJkAutoCheck->setChecked(recorder->isJackAutoMode());
    ui->optJktransCheck->setChecked(recorder->isJackTransMode());
    ui->optRecordAtLaunchCheck->setChecked(recorder->isRecordAtLaunch());

    connect(recorder, SIGNAL(statusChanged()), this, SLOT(onRecorderStatusChanged()));
}

MainWindow::~MainWindow()
{
    delete iconGreen;
    delete iconRed;
    delete iconOrange;
    delete ui;
}


//=============================================================================
// Events methods
//=============================================================================

void MainWindow::on_recButton_clicked()
{    
    recorder->setRecording(!recorder->isRecording());
}

void MainWindow::on_pauseDelaySpin_valueChanged(double secs)
{
    recorder->setPauseActivationDelay(secs);
}

void MainWindow::on_pauseLevelSpin_valueChanged(double level)
{
    recorder->setPauseLevel(level);
    ui->vuMeter->setCompLevel(level);
}

void MainWindow::onRecorderStatusChanged()
{
    if (recorder->isShutdown()) {
        close();
    }
    else {
        ui->vuMeter->setLeftLevel(recorder->getLeftLevel());
        ui->vuMeter->setRightLevel(recorder->getRightLevel());

        if (recorder->getDiskSpace() >= 0) {
            ui->recDiskProgress->setValue(recorder->getDiskSpace());
            ui->recDiskProgress->setFormat("%p%");
        }
        else {
            ui->recDiskProgress->setValue(100);
            ui->recDiskProgress->setFormat(tr("Invalid dir"));
        }

        if (ui->recFileEdit->text() != recorder->getCurrentFilePath())
            ui->recFileEdit->setText(recorder->getCurrentFilePath());

        if (ui->postLastEdit->text() != recorder->getProcessFilePath())
            ui->postLastEdit->setText(recorder->getProcessFilePath());

        QString statusMessage;
        QTextStream strm(&statusMessage);
        if (recorder->isRecording()) {
            ui->recButton->setEnabled(true);
            if (recorder->isPaused()) {
                ui->recButton->setIcon(*iconOrange);
                strm << tr("Waiting for sound...");
            }
            else {
                ui->recButton->setIcon(*iconRed);
                strm << tr("Recording...");
            }
        }
        else if (!recorder->isRecordEnabled()) {
            ui->recButton->setEnabled(false);
            strm << tr("Disabled");
        }
        else {
            ui->recButton->setEnabled(true);
            ui->recButton->setIcon(*iconGreen);
            strm << tr("Ready");
        }
        strm << " "
             << recorder->getCurrentRecordSize()/1024 << "KB"
             << " [" << toTimeText(recorder->getCurrentRecordTimeSecs()) << "]"
             << " - "
             << recorder->getTotalRecordSize()/1024 << "KB"
             << " [" << toTimeText(recorder->getTotalRecordTimeSecs()) << "]";
        ui->statusBar->showMessage(statusMessage);
    }
}

void MainWindow::on_pauseSplitCheck_stateChanged(int value)
{
    recorder->setSplitMode(value != 0);
}

void MainWindow::on_postActionCombo_currentIndexChanged(int index)
{
    ui->postCmdEdit->setText(ui->postActionCombo->itemData(index).toString());
}

void MainWindow::on_postCmdEdit_textChanged(const QString &text)
{
    recorder->setProcessCmdLine(text);
    int postItem = ui->postActionCombo->findData(recorder->getProcessCmdLine());
    if (postItem >= 0)
        ui->postActionCombo->setCurrentIndex(postItem);
    else {
        ui->postActionCombo->setItemData(POSTCMD_CUSTOM_ITEM, recorder->getProcessCmdLine());
        ui->postActionCombo->setCurrentIndex(POSTCMD_CUSTOM_ITEM);
    }
}

void MainWindow::on_optOutputDirEdit_textChanged(const QString &text)
{
    recorder->setOutputDir(QDir(text));
}

void MainWindow::on_optJkAutoCheck_stateChanged(int value)
{
    recorder->setJackAutoMode(value != 0);
}

void MainWindow::on_optJktransCheck_stateChanged(int value)
{
    recorder->setJackTransMode(value != 0);
}

void MainWindow::on_optRecordAtLaunchCheck_stateChanged(int value)
{
    recorder->setRecordAtLaunch(value != 0);
}

void MainWindow::on_optOutputDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),ui->optOutputDirEdit->text(),QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty())
        ui->optOutputDirEdit->setText(dir);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    recorder->setRecording(false);
    event->accept();
}

QString MainWindow::toTimeText(long secs)
{
    // cannot use QTime because it wraps after 24h
    return QString("%1:%2:%3").arg(secs/3600,2,10,QLatin1Char('0')).arg((secs/60)%60,2,10,QLatin1Char('0')).arg(secs%60,2,10,QLatin1Char('0'));
}

