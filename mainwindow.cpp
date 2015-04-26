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

    readSettings();

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

        if (recorder->isRecording()) {
            ui->recButton->setEnabled(true);
            if (recorder->isPaused()) {
                ui->recButton->setIcon(*iconOrange);
                ui->statusBar->showMessage(tr("Waiting for sound..."));
            }
            else {
                ui->recButton->setIcon(*iconRed);
                ui->statusBar->showMessage(tr("Recording..."));
            }
        }
        else if (!recorder->isRecordEnabled()) {
            ui->recButton->setEnabled(false);
            ui->statusBar->showMessage(tr("Disabled"));
        }
        else {
            ui->recButton->setEnabled(true);
            ui->recButton->setIcon(*iconGreen);
            ui->statusBar->showMessage(tr("Ready"));
        }
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

void MainWindow::on_optOutputDirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory"),ui->optOutputDirEdit->text(),QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty())
        ui->optOutputDirEdit->setText(dir);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

//=============================================================================
// Settings methods
//=============================================================================

void MainWindow::writeSettings()
{
    QSettings settings("qjackrcd", "qjackrcd");

    settings.beginGroup("Recorder");
    settings.setValue("pauseLevel", recorder->getPauseLevel());
    settings.setValue("pauseActivationDelay", recorder->getPauseActivationDelay());
    settings.setValue("splitMode", recorder->isSplitMode());
    settings.setValue("processCmdLine", recorder->getProcessCmdLine());
    settings.setValue("connections1", recorder->getJackConnections1());
    settings.setValue("connections2", recorder->getJackConnections2());
    settings.setValue("jackAuto", recorder->isJackAutoMode());
    settings.setValue("jackTrans", recorder->isJackTransMode());
    settings.setValue("outputDir", recorder->getOutputDir().absolutePath());

    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("qjackrcd", "qjackrcd");

    settings.beginGroup("Recorder");
    recorder->setPauseLevel(settings.value("pauseLevel", -20).toFloat());
    recorder->setPauseActivationDelay(settings.value("pauseActivationDelay", 3).toInt());
    recorder->setSplitMode(settings.value("splitMode", false).toBool());
    recorder->setProcessCmdLine(settings.value("processCmdLine", "").toString());
    recorder->setJackConnections1(settings.value("connections1", "").toString());
    recorder->setJackConnections2(settings.value("connections2", "").toString());
    recorder->setJackAutoMode(settings.value("jackAuto", true).toBool());
    recorder->setJackTransMode(settings.value("jackTrans", true).toBool());
    recorder->setOutputDir(settings.value("outputDir", QDir::home().absolutePath()).toString());

    settings.endGroup();
}


