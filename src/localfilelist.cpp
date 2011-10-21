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


#include <QMap>
#include <QMapIterator>
#include <QPointer>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "localfilelist.h"
#include "file.h"
#include "filelist.h"
#include "filepirate.h"

LocalFileList::LocalFileList(QObject *parent) :
    QObject(parent)
{
    bool generateSchema = QFile::exists(FilePirate::StoragePath+"local-barrel.rum");
    db = new QSqlDatabase();
    db->addDatabase("SQLITE");
    db->setDatabaseName(FilePirate::StoragePath+"local-barrel.rum");
    if (db->open() && generateSchema) {
        qWarning() << "Could not open local barrel or barrel is new";
        FileList::GenerateSchema(db);
    } else {
        // we should throw an error here

    }
}

void LocalFileList::addFile(QString path)
{

}

void LocalFileList::clearFolder(QString sharedFolder)
{

    emit fileListChanged();
    emit shareSizeChanged(listSize);
}

void LocalFileList::clear()
{

}

void LocalFileList::removeFile(uint32_t id)
{
    db->exec("DELETE FROM files WHERE id="+QString::number(id));
}

File* LocalFileList::getFile(uint32_t id)
{
    QSqlQuery result = db->exec("SELECT FROM files WHERE id="+QString::number(id));
    if (!result.first())
        return 0;
    return File::fromQuery(result);
}

void LocalFileList::updateFile(uint32_t id)
{

}
