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
#include <QXmlStreamWriter>

FilePirate::FilePirate()
{
    maximumDownload = 0;
    maximumUpload = 0;
    maxDownloadSlots = 5;
    maxUploadSlots = 5;

    // TODO: load the existing filelist if it exists
    this->localFileList = new FileList(this);
    this->fileMon = new LocalFileMonitor(this);
    this->fileMonitorThread = new QThread(this);

    // Connect all our signals and slots
    connect(this->localFileList, SIGNAL(shareSizeChanged(ulong)), this, SLOT(localShareSizeChanged(ulong)));
    connect(this->localFileList, SIGNAL(fileListChanged()), this, SLOT(localFileListChanged()));
    connect(this, SIGNAL(settingsChanged()), this->fileMon, SLOT(updateWatchPaths()));
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

void FilePirate::saveSettings()
{
    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter();
    QString fileloc = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QFile f(fileloc + "/fpconfig.xml");
    f.open(QIODevice::WriteOnly);
    xmlWriter->setDevice(&f);
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("settings");
    // User information
    xmlWriter->writeStartElement("user");
    xmlWriter->writeAttribute("username",username);
    xmlWriter->writeAttribute("announce",QString(announceAsAdmin?"true":"false"));
    if (announceAsAdmin)
        xmlWriter->writeAttribute("key",announceKey);
    xmlWriter->writeEndElement();
    // Limit information
    xmlWriter->writeStartElement("limits");
    xmlWriter->writeAttribute("download",QString::number(maximumDownload));
    xmlWriter->writeAttribute("upload",QString::number(maximumUpload));
    xmlWriter->writeEndElement();
    // Slot information
    xmlWriter->writeStartElement("slots");
    xmlWriter->writeAttribute("download",QString::number(maxDownloadSlots));
    xmlWriter->writeAttribute("upload",QString::number(maxUploadSlots));
    xmlWriter->writeEndElement();
    // Shared folder information
    xmlWriter->writeStartElement("shared-folders");
    for (int i = 0; i < sharedFolders.size(); ++i)
    {
        xmlWriter->writeStartElement("folder");
        xmlWriter->writeAttribute("path",sharedFolders.at(i));
        xmlWriter->writeEndElement();
    }
    xmlWriter->writeEndElement();
    // End the settings element
    xmlWriter->writeEndElement();
    // End document and write
    xmlWriter->writeEndDocument();
    delete xmlWriter;
    emit settingsChanged();
}

ulong FilePirate::str2long(const char *s)
{
    ulong l;
    char *end;
    errno = 0;
    l = strtoul(s, &end, 0);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return 0;
    }
    if (*s == '\0' || *end != '\0') {
        return 0;
    }
    return l;
}

bool FilePirate::loadSettings()
{
    QString fileloc = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDomDocument doc;
    QFile f(fileloc + "/fpconfig.xml");
    if (!f.exists()) return false;
    if (!f.open(QIODevice::ReadOnly)) return false;
    if (!doc.setContent(&f))
    {
        f.close();
        return false;
    }
    f.close();
    QDomNodeList nodes = doc.documentElement().childNodes();
    if (nodes.count() == 0)
        return false;
    for (int i = 0; i < nodes.count(); i++)
    {
        if (nodes.at(i).nodeName().toUpper() == "USER") {
            username = nodes.at(i).attributes().namedItem("username").nodeValue();
            announceAsAdmin = (nodes.at(i).attributes().namedItem("announce").nodeValue() == "true");
            announceKey = nodes.at(i).attributes().namedItem("key").nodeValue();
        } else if (nodes.at(i).nodeName().toUpper() == "LIMITS") {
            maximumDownload = str2long((const char *)nodes.at(i).attributes().namedItem("download").nodeValue().toLocal8Bit().data());
            maximumUpload = str2long((const char *)nodes.at(i).attributes().namedItem("upload").nodeValue().toLocal8Bit().data());
        } else if (nodes.at(i).nodeName().toUpper() == "SLOTS") {
            maxDownloadSlots = str2long((const char *)nodes.at(i).attributes().namedItem("download").nodeValue().toLocal8Bit().data());
            maxUploadSlots = str2long((const char *)nodes.at(i).attributes().namedItem("upload").nodeValue().toLocal8Bit().data());
        } else if (nodes.at(i).nodeName().toUpper() == "SHARED-FOLDERS") {
            for (int i = 0; i < nodes.at(i).childNodes().count(); ++i)
            {
                if (nodes.at(i).childNodes().at(i).nodeName().toUpper() == "FOLDER")
                    sharedFolders.append(nodes.at(i).childNodes().at(i).namedItem("path").nodeValue());
            }
        }
    }

    fileMon->startTimer();
    return true;
}

void FilePirate::moveHelpersToThreads()
{
    this->fileMon->moveToThread(this->fileMonitorThread);
}
