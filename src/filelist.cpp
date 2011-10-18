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


#include <QMap>
#include <QMapIterator>
#include <QPointer>

#include "filelist.h"
#include "file.h"

FileList::FileList(QObject *parent) :
    QObject(parent)
{
    files = QMap<QString, QPointer<File> >();
}

void FileList::addFile(QString sharedName, QString path)
{
    files[sharedName] = &File(this, path);
}

void FileList::clearFolder(QString sharedFolder)
{
    QMapIterator<QString, QPointer<File> > i(files);
    while (i.hasNext())
    {
        i.next();
        if (i.key().startsWith(sharedFolder))
        {
            listSize -= i.value()->getFileSize();
            files.remove(i.key());
        }
    }
    emit fileListChanged();
    emit shareSizeChanged(listSize);
}

void FileList::clear()
{
    files.clear();
}

void FileList::removeFile(QString sharedName)
{
    files.remove(sharedName);
}

File* FileList::getFile(QString sharedName)
{
    return files[sharedName];
}


void FileList::updateFile(QString sharedName)
{
    files[sharedName]->forceUpdate();
}
