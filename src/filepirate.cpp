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


#include "filepirate.h"
#include <QDir>
#include <QDesktopServices>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

FilePirate::FilePirate()
{
    maximumDownload = 0;
    maximumUpload = 0;
    maxDownloadSlots = 5;
    maxUploadSlots = 5;

    connect(&this->localFileList, SIGNAL(shareSizeChanged(ulong)), this, SLOT(localShareSizeChanged(ulong)));
    connect(&this->localFileList, SIGNAL(fileListChanged()), this, SLOT(localFileListChanged()));
}

void FilePirate::localFileListChanged()
{

}

void FilePirate::localShareSizeChanged(ulong size)
{

}

void FilePirate::handleUrl(const QUrl &url)
{
    qWarning("f: FilePirate::handleUrl %s", url.toString());
}

bool FilePirate::loadSettings()
{
    QString fileloc = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDomDocument doc("configuration");
    QFile f(fileloc + "/fpconfig.xml");
    if (!f.exists()) return false;
    if (!f.open(QIODevice::ReadOnly)) return false;
    if (!doc.setContent(&f))
    {
        f.close();
        return false;
    }
    f.close();
    QDomElement docEl = doc.documentElement();
    QDomNodeList nodes = docEl.elementsByTagName("settings").at(0).childNodes();
    if (nodes.count() == 0)
        return false;
    for (int i = 0; i < nodes.count(); i++)
    {
        if (nodes.at(i).nodeName().toUpper() == "USER") {
            username = nodes.at(i).attributes().namedItem("username").nodeValue();
        } else if (nodes.at(i).nodeName().toUpper() == "LIMITS") {
            maximumDownload = nodes.at(i).attributes().namedItem("download").nodeValue().toInt();
            maximumUpload = nodes.at(i).attributes().namedItem("upload").nodeValue().toInt();
        } else if (nodes.at(i).nodeName().toUpper() == "SLOTS") {
            maxDownloadSlots = nodes.at(i).attributes().namedItem("download").nodeValue().toInt();
            maxUploadSlots = nodes.at(i).attributes().namedItem("upload").nodeValue().toInt();
        } else {
            return false;
        }
    }
    nodes = docEl.elementsByTagName("shared-folders").at(0).toElement().elementsByTagName("folder");
    if (nodes.count() > 0)
        for (int i = 0; i < nodes.count(); ++i)
            sharedFolders.append(nodes.at(i).attributes().namedItem("path").nodeValue());
    fileMon.startTimer();
    fileMon.refreshFileList();
    return true;
}
