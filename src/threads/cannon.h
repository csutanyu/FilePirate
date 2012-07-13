/*
* Copyright (c) 2012 Jonathan W Enzinna <jonnyfunfun@jonnyfunfun.com>
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

#ifndef CANNON_H
#define CANNON_H

#include <QObject>
#include <QFile>
#include <QtNetwork>
#include <QtNetwork/QTcpServer>
#include "../file.h"

class Cannon : public QObject
{
    Q_OBJECT
public:
    explicit Cannon(QObject *parent = 0, QHostAddress receiver = 0, int port = 0, QFile file = 0);
signals:
    void progressUpdate(long total, long sent);
    void complete();
    void error();
public slots:        
    void sendFile();
private:
    QHostAddress receiver;
    int port;
    File* file;
    QTcpServer* socket;
};

#endif // CANNON_H
