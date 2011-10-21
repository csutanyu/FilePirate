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

#include "filepirate.h"
#include "defines.h"
#include <QDir>
#include <QDesktopServices>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QXmlStreamWriter>

QString FilePirate::StoragePath = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + DIRECTORY_SEPARATOR + "FilePirate" + DIRECTORY_SEPARATOR;

FilePirate::FilePirate()
{
    // Defaults
    maximumDownload = 0;
    maximumUpload = 0;
    maxDownloadSlots = 5;
    maxUploadSlots = 5;
    allocateAllDownloads = false;
    appendUploadingUsername = false;
    enableAVIntegration = false;
    overridePreferredHash = false;
    announceAsAdmin = false;
    defaultDownloadPath = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation) + DIRECTORY_SEPARATOR + "Downloads";

    if (!QDir(StoragePath).exists())
        QDir().mkdir(StoragePath);

    // TODO: load the existing filelist if it exists
    this->localList = new LocalFileList(this);
    this->fileMon = new LocalFileMonitor();
    this->fileMonitorThread = new QThread(this);

    // Connect all our signals and slots
    connect(this->localList, SIGNAL(shareSizeChanged(ulong)), this, SLOT(localShareSizeChanged(ulong)));
    connect(this->localList, SIGNAL(fileListChanged()), this, SLOT(localListChanged()));
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
    qDebug() << "FilePirate::handleUrl " << url.toString();
}

void FilePirate::saveSettings()
{
    qDebug("Saving settings...");
    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter();
    QFile f(StoragePath + "/config.xml");
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
    // Download defaults
    xmlWriter->writeStartElement("downloads");
    xmlWriter->writeAttribute("default-path",defaultDownloadPath);
    xmlWriter->writeAttribute("preallocate",QString(allocateAllDownloads?"true":"false"));
    xmlWriter->writeAttribute("append-username",QString(appendUploadingUsername?"true":"false"));
    xmlWriter->writeEndElement();
    // AV Integration
    xmlWriter->writeStartElement("av");
    xmlWriter->writeAttribute("enabled",QString(enableAVIntegration?"true":"false"));
    xmlWriter->writeAttribute("path",avExPath);
    xmlWriter->writeEndElement();
    // Shared folder information
    xmlWriter->writeStartElement("shared-folders");
    QMapIterator<QString, QString> i(sharedFolders);
    while (i.hasNext())
    {
        i.next();
        xmlWriter->writeStartElement("folder");
        xmlWriter->writeAttribute("path",i.value());
        xmlWriter->writeAttribute("name",i.key());
        xmlWriter->writeEndElement();
    }
    xmlWriter->writeEndElement();
    // End the settings element
    xmlWriter->writeEndElement();
    // End document and write
    xmlWriter->writeEndDocument();
    delete xmlWriter;
    qDebug("DONE!");
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
    QDomDocument doc;
    QFile f(StoragePath + "/config.xml");
    if (!f.exists()) return false;
    if (!f.open(QIODevice::ReadOnly)) return false;
    if (!doc.setContent(&f))
    {
        f.close();
        return false;
    }
    f.close();
    QDomNodeList nodes = doc.documentElement().childNodes();
    if (nodes.count() == 0) {
        qWarning() << "Cannot load settings - no nodes found in document";
        return false;
    }
    qDebug("Loading settings...");
    for (int i = 0; i < nodes.count(); i++)
    {
        if (nodes.at(i).nodeName().toUpper() == "USER") {
            qDebug("Processing user information...");
            username = nodes.at(i).attributes().namedItem("username").nodeValue();
            announceAsAdmin = (nodes.at(i).attributes().namedItem("announce").nodeValue().toLower() == "true");
            announceKey = nodes.at(i).attributes().namedItem("key").nodeValue();
        } else if (nodes.at(i).nodeName().toUpper() == "LIMITS") {
            qDebug("Processing limits...");
            maximumDownload = str2long((const char *)nodes.at(i).attributes().namedItem("download").nodeValue().toLocal8Bit().data());
            maximumUpload = str2long((const char *)nodes.at(i).attributes().namedItem("upload").nodeValue().toLocal8Bit().data());
        } else if (nodes.at(i).nodeName().toUpper() == "SLOTS") {
            qDebug("Processing slots...");
            maxDownloadSlots = str2long((const char *)nodes.at(i).attributes().namedItem("download").nodeValue().toLocal8Bit().data());
            maxUploadSlots = str2long((const char *)nodes.at(i).attributes().namedItem("upload").nodeValue().toLocal8Bit().data());
        } else if (nodes.at(i).nodeName().toUpper() == "DOWNLOADS") {
            qDebug("Processing downloads...");
            defaultDownloadPath = nodes.at(i).attributes().namedItem("default-path").nodeValue();
            appendUploadingUsername = (nodes.at(i).attributes().namedItem("append-username").nodeValue().toLower() == "true");
            allocateAllDownloads = (nodes.at(i).attributes().namedItem("preallocate").nodeValue().toLower() == "true");
        } else if (nodes.at(i).nodeName().toUpper() == "AV") {
            qDebug("Processing AV settings...");
            enableAVIntegration = (nodes.at(i).attributes().namedItem("enabled").nodeValue().toLower() == "true");
            avExPath = nodes.at(i).attributes().namedItem("path").nodeValue();
        } else if (nodes.at(i).nodeName().toUpper() == "SHARED-FOLDERS") {
            qDebug("Processing "+QString::number(nodes.at(i).childNodes().count()).toAscii()+" shared folder entries...");
            for (int c = 0; c < nodes.at(i).childNodes().count(); ++c)
            {
                if (nodes.at(i).childNodes().at(c).nodeName().toUpper() == "FOLDER")
                {
                    sharedFolders[nodes.at(i).childNodes().at(c).attributes().namedItem("name").nodeValue()] = nodes.at(i).childNodes().at(c).attributes().namedItem("path").nodeValue();
                    qDebug("Sharing folder "+nodes.at(i).childNodes().at(c).attributes().namedItem("name").nodeValue().toAscii()+" at "+nodes.at(i).childNodes().at(c).attributes().namedItem("path").nodeValue().toAscii());
                }
            }
        }
    }

    qDebug("Done loading settings");

    fileMon->startTimer();
    return true;
}

void FilePirate::moveHelpersToThreads()
{
    this->fileMon->moveToThread(this->fileMonitorThread);
}
