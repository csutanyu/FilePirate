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


#include "localfilemonitor.h"
#include <QTimer>
#include <QFileSystemWatcher>
#include <QFuture>
#include <QtConcurrentRun>

#include "localfilelist.h"
#include "filepirate.h"

LocalFileMonitor::LocalFileMonitor(QObject *parent) :
    QObject(parent)
{
    this->refreshTimer.start(30000000);
    this->fsWatch = new QFileSystemWatcher(this);
    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(refreshTimerEvent()));
    connect(this->fsWatch, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
    connect(this->fsWatch, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
}

void LocalFileMonitor::fileChanged(QString path)
{
    // Determine the shared folder to find the shared name

}

void LocalFileMonitor::directoryChanged(QString path)
{
    // Remove all from that folder and refresh
    QMapIterator<QString, QString> i(FilePirate::Application().sharedFolders);
    while (i.hasNext())
    {
        i.next();
        if (path.contains(i.value()))
        {
            FilePirate::Application().localList->clearFolder(i.key());
            break;
        }
    }
    fullRefreshFileList();
    emit refreshStarted();
}

void LocalFileMonitor::updateWatchPaths()
{
    this->fsWatch->removePaths(this->fsWatch->directories());
    QMapIterator<QString, QString> i(FilePirate::Application().sharedFolders);
    while (i.hasNext())
    {
        i.next();
        this->fsWatch->addPath(i.value());
    }
}

void LocalFileMonitor::refreshTimerEvent()
{
    this->fullRefreshFileList();
}

void LocalFileMonitor::fullRefreshFileList()
{
    emit refreshStarted();
    refreshTimer.stop();
    // Refresh the file list

    refreshTimer.start();
    emit refreshCompleted();
}

void LocalFileMonitor::startTimer()
{
    refreshTimer.start();
}
