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

#ifndef OCEANSOCKET_H
#define OCEANSOCKET_H

#include <QObject>
#include <QTimer>
#include <QtNetwork/QUdpSocket>

class OceanSocket : public QObject
{
    Q_OBJECT
public:
    explicit OceanSocket(QObject *parent = 0);
    bool sendMessage(const char message_type, QString data);
signals:
    void shipHeartbeat(QHostAddress host, QString username);
    void shipChangedCourse(QHostAddress host, QString username);
    void messageReceived(QHostAddress host, QString username, QString message);
    void adminLockRequested(QHostAddress host, QString key);
    void adminLockRequestReponse(QHostAddress host, bool successful, QString key);
public slots:
    void broadcastChanges();
private slots:
    void heartbeatTimer();
    void processIncomingWaves();
private:
    QTimer *heartbeat;
    QUdpSocket *socket;

    void processMessage(QHostAddress host, QString raw_message);
};

#endif // OCEANSOCKET_H
