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

#include "fpmcsocket.h"
#include <QtNetwork/QUdpSocket>
#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#ifdef Q_OS_LINUX
#endif

FPMcSocket::FPMcSocket()
{
    McSocket.bind(33333,QUdpSocket::ShareAddress);

    connect(&McSocket,SIGNAL(readyRead()),this,SLOT(FPMc_readNetPacket()));
}

void FPMcSocket::sendMulticast(QString message)
{
    sendto(McSocketId, message.toAscii(), message.toAscii().size(), 0,
           (struct sockaddr*)&McSinStruct, sizeof(McSinStruct));
}

void FPMcSocket::FPMc_readNetPacket()
{
    QString OInMsg;
    QHostAddress Peer;
    quint16 port = 33333;
    while (McSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(McSocket.pendingDatagramSize());
        McSocket.readDatagram(datagram.data(),datagram.size(),&Peer,&port);
        OInMsg = datagram.data();
        // process the datagram
    }
}
