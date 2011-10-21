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


#include "src/aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    ui->buildDTLabel->setText("Built on " + QString::fromLocal8Bit(__DATE__)
                              + " at " + QString::fromLocal8Bit(__TIME__));
    QString versionString = QString("%1.%2.%3")
            .arg((FILEPIRATE_VERSION & 0xFF0000) >> 16,	2, 16, QChar('0'))
            .arg((FILEPIRATE_VERSION & 0x00FF00) >> 8,	2, 16, QChar('0'))
            .arg((FILEPIRATE_VERSION & 0x0000FF),	2, 16, QChar('0'));
    ui->versionLabel->setText("Version " + versionString);

    connect(ui->awesomeButton, SIGNAL(clicked()), this, SLOT(closeWindow()));

    this->setWindowFlags(Qt::WindowFlags(Qt::Dialog));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::closeWindow()
{
    this->close();
}
