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
#include <QMutex>
#include <QWaitCondition>

#ifndef QT_DEBUG
#include <QDateTime>
#include <QtDebug>
#include <QFile>
#include <QIODevice>

QTextStream *out = 0;

void logOutput(QtMsgType type, const char *msg)
{
    QString debugdate = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    switch (type)
    {
    case QtDebugMsg:
        debugdate += "[D]";
        break;
    case QtWarningMsg:
        debugdate += "[W]";
        break;
    case QtCriticalMsg:
        debugdate += "[C]";
        break;
    case QtFatalMsg:
        debugdate += "[F]";
    }
    (*out) << debugdate << " " << msg << endl;

    if (QtFatalMsg == type)
    {
        abort();
    }
}
#endif

int main(int argc, char *argv[])
{
    QSharedMemory sharedMemory("FilePirateClient");
    if (!sharedMemory.create(1) || sharedMemory.error() == QSharedMemory::AlreadyExists)
        return 0;

#ifndef QT_DEBUG
    QString fileName = FilePirate::StoragePath + "captains-log.log";
    QFile *log = new QFile(fileName);
    if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        out = new QTextStream(log);
        qInstallMsgHandler(logOutput);
    } else {
        qDebug() << "Error opening log file '" << fileName << "'. All debug output redirected to console.";
    }
#endif

    QApplication a(argc, argv);

    QPixmap pixmap(":/images/splash.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    a.processEvents();
    splash->showMessage("Loading...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    qDebug() << "Registering URL handler...";
    QDesktopServices::setUrlHandler("pirate",&FilePirate::Application(),"handleUrl");
    a.processEvents();
    // Force the user to see the splash screen for at least some time
    QMutex dummy;
    dummy.lock();
    QWaitCondition wait;
    wait.wait(&dummy, 750);
    dummy.unlock();
    // And move on to the rest of loading
    splash->showMessage("Loading settings...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    // load the settings here
    FilePirate::Application().settingsLoaded = FilePirate::Application().loadSettings();
    splash->showMessage("Loading local filelist...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    FilePirate::Application().fileMon->fullRefreshFileList();

    // bring up the filelist - this is a good time to start up the settings dialog
    // if the user hasn't done so yet...
    splash->showMessage("Finishing up...",Qt::AlignCenter | Qt::AlignBottom,Qt::white);
    FilePirate::Application().moveHelpersToThreads();

    MainWindow w;
    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
