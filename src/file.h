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

#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QByteArray>
#include <QSqlQuery>

class File : public QObject
{
    Q_OBJECT
public:
    explicit File(QObject *parent = 0, const uint32_t id = 0, const QString path = 0, const QString share = 0, const QByteArray hash = 0);
    static File* fromQuery(QSqlQuery q);
    QByteArray getHashValue();
    QString getFilePath();
    uint64_t getFileSize();
    void forceUpdate();
signals:

public slots:
private:
    QString filePath;
    QString shareName;
    QByteArray hashValue;
    uint64_t fileSize;
    uint32_t fileId;
};

#endif // FILE_H
