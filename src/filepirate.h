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

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <QThread>

#include "filelist.h"
#include "localfilemonitor.h"

#ifndef FILEPIRATE_H
#define FILEPIRATE_H

class FilePirate : public QObject
{
    Q_OBJECT

public:
    static FilePirate& Application()
    {
        static FilePirate instance;
        return instance;
    }
    void saveSettings();
    bool loadSettings();
    void moveHelpersToThreads();

    LocalFileMonitor* fileMon;
    FileList* localFileList;
    QStringList sharedFolders;
    QString username;
    QString defaultDownloadPath;
    bool announceAsAdmin;
    QString announceKey;
    bool overridePreferredHash;
    uint16_t overriddenHashAlgorithm;
    bool enableAVIntegration;
    QString avExPath;
    uint16_t maxUploadSlots;
    uint16_t maxDownloadSlots;
    ulong maximumUpload;
    ulong maximumDownload;
    bool settingsLoaded;
signals:
    void settingsChanged();
public slots:
    void handleUrl(const QUrl &url);
private slots:
    void localFileListChanged();
    void localShareSizeChanged(ulong size);
private:
    ulong str2long(const char *s);
    FilePirate();
    // unused for the pattern - don't implement
    FilePirate(FilePirate const&);
    void operator=(FilePirate const&);
    QThread* fileMonitorThread;
};

#endif // FILEPIRATE_H
