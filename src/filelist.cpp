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


#include "filelist.h"
#include "file.h"

FileList::FileList(QObject *parent) :
    QObject(parent)
{
}

void FileList::addFile(QString path)
{
    File newFile(this,path);
    listSize += newFile.getFileSize();
    files.append(newFile);
    emit fileListChanged();
    emit shareSizeChanged(listSize);
}

void FileList::clear()
{
    files.removeAll();
    listSize = 0;
    emit fileListChanged();
    emit shareSizeChanged(listSize);
}

void FileList::removeFile(int index)
{
    File f = files.at(index);
    listSize -= f.getFileSize();
    files.removeAt(index);
    emit fileListChanged();
    emit shareSizeChanged(listSize);
}

File FileList::getFile(int index)
{
    return files.at(index);
}

File FileList::find(QString needle)
{
    for (int i = 0; i < files.count(); ++i)
    {
        if (files.at(i).getFilePath().contains(needle, Qt::CaseInsensitive))
            return files.at(i);
    }
    return 0;
}

File FileList::get(QString path)
{
    for (int i = 0; i < files.count(); ++i)
    {
        if (files.at(i).getFilePath().toUpper() == path.toUpper())
            return files.at(i);
    }
    return 0;
}

void FileList::updateFile(QString path)
{
    File f = get(path);
    if (f != 0)
    {
        listSize -= f.getFileSize();
        // Update
        f.forceUpdate();
        listSize += f.getFileSize();
        emit shareSizeChanged(listSize);
    }
}
