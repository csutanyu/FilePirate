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

#include "file.h"
#include "defines.h"
#include "hash/filehasher.h"

#include <QMap>
#include <QStringList>
#include <QFile>
#include <QVariant>
#include <QSqlRecord>


File::File(QObject *parent, uint32_t id, QString path, QString share, QByteArray hash) :
    QObject(parent)
{
    this->filePath = path;
    if (hash.isEmpty())
        this->hashValue = FileHasher::hashFile(path);
    else
        this->hashValue = hash;
    this->shareName = share;
    this->fileId = id;
    QFile fh;
    if (fh.open(QIODevice::ReadOnly))
    {
        this->fileSize = fh.size();
        fh.close();
    }
}

File* File::fromQuery(QSqlQuery q)
{
    uint32_t id = q.value(0).toInt();
    QString share = q.value(q.record().indexOf("shareName")).toString();
    QString path = q.value(q.record().indexOf("filePath")).toString();
    QByteArray hash = q.value(q.record().indexOf("fileHash")).toByteArray();
    return new File(0,id,path,share,hash);
}

uint64_t File::getFileSize()
{
    return this->fileSize;
}

void File::forceUpdate()
{
    this->hashValue = FileHasher::hashFile(filePath);
    QFile fh;
    if (fh.open(QIODevice::ReadOnly))
    {
        this->fileSize = fh.size();
        fh.close();
    }
}

QString File::getFilePath()
{
    return this->filePath;
}
