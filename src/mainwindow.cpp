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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include <QMessageBox>
#include "filepirate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connect signals and shit
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitClicked()));
    connect(ui->actionAbout_FilePirate,SIGNAL(triggered()),this,SLOT(aboutClicked()));
    connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(settingsClicked()));
    connect(ui->actionRefreshFileList, SIGNAL(triggered()), FilePirate::Application().fileMon, SLOT(refreshTimerEvent()));

    if (!FilePirate::Application().settingsLoaded)
    {
        settingsWindow.setParent(this, Qt::WindowFlags(Qt::Dialog));
        settingsWindow.show();
        settingsWindow.raise();
        settingsWindow.activateWindow();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exitClicked()
{
    if (QMessageBox::question(this,"Really quit?","Are you sure you want to quit?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            this->close();
}

void MainWindow::aboutClicked()
{
    aboutWindow.setParent(this, Qt::WindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint));
    aboutWindow.show();
    aboutWindow.raise();
    aboutWindow.activateWindow();
}

void MainWindow::settingsClicked()
{
    settingsWindow.setParent(this, Qt::WindowFlags(Qt::Dialog));
    settingsWindow.show();
    settingsWindow.raise();
    settingsWindow.activateWindow();
}
