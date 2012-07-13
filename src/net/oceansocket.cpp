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

#include "oceansocket.h"
#include "../defines.h"
#include "../filepirate.h"

OceanSocket::OceanSocket(QObject *parent) :
    QObject(parent)
{
    // setup the heartbeat timer
    heartbeat = new QTimer(this);
    connect(heartbeat, SIGNAL(timeout()), this, SLOT(heartbeatTimer()));

    // setup our ocean socket
    socket = new QUdpSocket(this);
    socket->bind(8047, QUdpSocket::ShareAddress);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processIncomingWaves()));

}

void OceanSocket::broadcastChanges()
{
    this->sendMessage(NET_OCEAN_HEARTBEAT, FilePirate::Application().username);
}

void OceanSocket::heartbeatTimer()
{
    this->sendMessage(NET_OCEAN_HEARTBEAT, FilePirate::Application().username);
}

bool OceanSocket::sendMessage(const char message_type, QString data)
{
    if (socket->isOpen())
    {
        QByteArray pulse = QString(message_type+data).toUtf8();
        socket->writeDatagram(pulse.data(), pulse.size(), QHostAddress::Broadcast, 8047);
        return true;
    }
    return false;
}

void OceanSocket::processIncomingWaves()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray wave;
        QHostAddress host;
        wave.resize(socket->pendingDatagramSize());
        socket->readDatagram(wave.data(), wave.size(), &host);
        if (wave.startsWith(NET_OCEAN_HEARTBEAT))
            emit shipHeartbeat(host, wave.mid(1).data());
        else if (wave.startsWith(NET_OCEAN_COURSE_CHANGE))
            emit shipChangedCourse(host, wave.mid(1).data());
        else if (wave.startsWith(NET_OCEAN_ADMIN_LOCK_REQ))
            emit adminLockRequested(host, wave.mid(1).data());
        else if (wave.startsWith(NET_OCEAN_ADMIN_LOCK_RESP))
            emit adminLockRequestReponse(host, (int(wave.mid(1,1).data()) == NET_OCEAN_ACKNOWLEDGE), wave.mid(2).data());
        else if (wave.startsWith(NET_OCEAN_MESSAGE))
            this->processMessage(host, wave.mid(1).data());
    }
}

void OceanSocket::processMessage(QHostAddress host, QString raw_message)
{
    QStringList messageParts = raw_message.split(NET_OCEAN_MESSAGE_SEPARATOR);
    emit messageReceived(host, messageParts[0], messageParts[1]);
}
