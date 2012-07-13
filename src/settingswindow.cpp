/*
* Copyright (c) 2011 Jonathan W Enzinna <jonnyfunfun@jonnyfunfun.com>
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
#include "defines.h"
#include "fphash.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QMapIterator>
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
    connect(ui->addFolderButton, SIGNAL(clicked()), this, SLOT(addSharedFolder()));
    connect(ui->removeFolderButton, SIGNAL(clicked()), this, SLOT(removeSharedFolder()));

    // load settings in
    ui->usernameEdit->setText(FilePirate::Application().username);
    ui->uploadSlotSlider->setValue(FilePirate::Application().maxUploadSlots);
    ui->downloadSlotSlider->setValue(FilePirate::Application().maxDownloadSlots);
    ui->uploadSpeedSlider->setValue(FilePirate::Application().maximumUpload);
    ui->downloadSpeedSlider->setValue(FilePirate::Application().maximumDownload);
    ui->announceKeyEdit->setEnabled(ui->announceAdminCheck->checkState() == Qt::Checked);
    ui->enableAVIntCheck->setChecked(FilePirate::Application().enableAVIntegration);
    ui->avExeNameLabel->setText(FilePirate::Application().avExPath.section(DIRECTORY_SEPARATOR,-1));
    ui->defaultDownloadPathEdit->setText(FilePirate::Application().defaultDownloadPath);
    ui->preAllocateFilesCheck->setChecked(FilePirate::Application().allocateAllDownloads);
    ui->announceAdminCheck->setChecked(FilePirate::Application().announceAsAdmin);
    ui->announceKeyEdit->setText(FilePirate::Application().announceKey);

    QMapIterator<QString, QString> i(FilePirate::Application().sharedFolders);
    while (i.hasNext())
    {
        i.next();
        ui->sharedFoldersList->addItem(QString(i.key()+" : "+i.value()));
    }

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
    FilePirate::Application().announceAsAdmin = ui->announceAdminCheck->isChecked();
    FilePirate::Application().announceKey = ui->announceKeyEdit->text();
    // Download/upload speed limits
    FilePirate::Application().maximumDownload = ui->downloadSpeedSlider->value();
    FilePirate::Application().maximumUpload = ui->uploadSpeedSlider->value();
    // Slot limits
    FilePirate::Application().maxDownloadSlots = ui->downloadSlotSlider->value();
    FilePirate::Application().maxUploadSlots = ui->uploadSlotSlider->value();
    // Shared Folders
    FilePirate::Application().sharedFolders.clear();
    for (int i = 0; i < ui->sharedFoldersList->count(); ++i)
    {
        QStringList item = ui->sharedFoldersList->item(i)->text().split(" : ");
        FilePirate::Application().sharedFolders[item[0]] = item[1];
    }
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
    QDesktopServices::openUrl(QUrl("http://www.filepirate.us/documentation/announce-keys"));
}

void SettingsWindow::addSharedFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this,"Choose Folder",NULL,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    bool ok;
    QString name = QInputDialog::getText(this, "Name this share", "Share Name:", QLineEdit::Normal,
        dir.section(DIRECTORY_SEPARATOR,-1), &ok);
    if (ok)
    {
        ui->sharedFoldersList->addItem(QString(name+" : "+dir));
    }
}

void SettingsWindow::removeSharedFolder()
{
    if (ui->sharedFoldersList->selectedItems().length() == 0)
        return;

    delete ui->sharedFoldersList->takeItem(ui->sharedFoldersList->currentRow());
}
