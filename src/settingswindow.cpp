/*
* Copyright (c) 2010 Jonathan W Enzinna <jonnyfunfun@jonnyfunfun.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/


#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "filepirate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelSettings()));

    connect(ui->uploadSlotSlider, SIGNAL(valueChanged(int)), ui->uploadSlotLCD, SLOT(display(int)));
    connect(ui->uploadSpeedSlider, SIGNAL(valueChanged(int)), ui->uploadSpeedLCD, SLOT(display(int)));
    connect(ui->downloadSlotSlider, SIGNAL(valueChanged(int)), ui->downloadSlotLCD, SLOT(display(int)));
    connect(ui->downloadSpeedSlider, SIGNAL(valueChanged(int)), ui->downloadSpeedLCD, SLOT(display(int)));
    connect(ui->defaultDownloadBrowseBtn, SIGNAL(clicked()), this, SLOT(beginDownloadPathBrowse()));
    connect(ui->announceAdminCheck, SIGNAL(clicked(bool)), ui->announceKeyEdit, SLOT(setEnabled(bool)));
    connect(ui->announceKeyHelpLink, SIGNAL(clicked()), this, SLOT(launchAnnounceKeyHelpURL()));
    connect(ui->enableAVIntCheck, SIGNAL(clicked(bool)), ui->avIntGroupFrame, SLOT(setEnabled(bool)));
    connect(ui->overrideHashAlgoCheck, SIGNAL(clicked(bool)), ui->preferrredHashAlgorithm, SLOT(setEnabled(bool)));

    // load settings in
    ui->usernameEdit->setText(FilePirate::Application().username);
    ui->uploadSlotSlider->setValue(FilePirate::Application().maxUploadSlots);
    ui->downloadSlotSlider->setValue(FilePirate::Application().maxDownloadSlots);
    ui->uploadSpeedSlider->setValue(FilePirate::Application().maximumUpload);
    ui->downloadSpeedSlider->setValue(FilePirate::Application().maximumDownload);

    ui->announceKeyEdit->setEnabled(ui->announceAdminCheck->checkState() == Qt::Checked);
    ui->preferrredHashAlgorithm->setEnabled(ui->overrideHashAlgoCheck->checkState() == Qt::Checked);

    // reset chosen tab
    ui->tabWidget->setCurrentIndex(0);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::saveSettings()
{
    // verify settings are correct

    // write settings to the FilePirate::Application object and signal
    // it to write the XML
    FilePirate::Application().username = ui->usernameEdit->text();
    FilePirate::Application().announceAsAdmin = ui->announceAdminCheck->checkState();
    FilePirate::Application().announceKey = ui->announceKeyEdit->text();
    // Download/upload speed limits
    FilePirate::Application().maximumDownload = ui->downloadSpeedSlider->value();
    FilePirate::Application().maximumUpload = ui->uploadSpeedSlider->value();
    // Slot limits
    FilePirate::Application().maxDownloadSlots = ui->downloadSlotSlider->value();
    FilePirate::Application().maxUploadSlots = ui->uploadSlotSlider->value();
    // Signal to save
    FilePirate::Application().saveSettings();

    this->close();
}

void SettingsWindow::cancelSettings()
{
    this->close();
}

void SettingsWindow::beginDownloadPathBrowse()
{
    QString dir = QFileDialog::getExistingDirectory(this,"Choose Folder",NULL,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->defaultDownloadPathEdit->setText(dir);
}

void SettingsWindow::launchAnnounceKeyHelpURL()
{
    QDesktopServices::openUrl(QUrl("http://code.google.com/p/filepirate/wiki/WhatAreAnnounceKeys"));
}
