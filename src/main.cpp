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

#include <QtGui/QApplication>
#include <QtNetwork/QUdpSocket>
#include "mainwindow.h"
#include "filepirate.h"
#include "settingswindow.h"
#include <QDesktopServices>
#include <QSplashScreen>
#include <QTimer>
#include <QThread>
#include <QPixmap>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QSharedMemory sharedMemory("FilePirateClient");
    if (!sharedMemory.create(1) || sharedMemory.error() == QSharedMemory::AlreadyExists)
        return 0;

    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap::fromImage(QImage::fromData(":/images/splash.png")));
    splash->show();

    splash->showMessage("Loading...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    QDesktopServices::setUrlHandler("pirate",&FilePirate::Application(),"handleUrl");
    a.processEvents();
    splash->showMessage("Loading settings...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    // load the settings here
    FilePirate::Application().settingsLoaded = FilePirate::Application().loadSettings();

    splash->showMessage("Loading local filelist...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    // bring up the filelist - this is a good time to start up the settings dialog
    // if the user hasn't done so yet...


    MainWindow w;
    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
