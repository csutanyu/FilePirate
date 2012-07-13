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

#include "adminlock.h"
#include "../filepirate.h"
#include "../defines.h"
#include <QTimer>

QString AdminLock::lockKey = "";
bool AdminLock::clientHasLock = false;

AdminLock::AdminLock(QObject *parent) :
    QObject(parent)
{
}

void AdminLock::handleLockRequest(QHostAddress host, QString key)
{
    if (key == AdminLock::lockKey)
        FilePirate::Application().ocean->sendMessage(NET_OCEAN_ADMIN_LOCK_RESP, AdminLock::lockKey.prepend(NET_OCEAN_ACKNOWLEDGE));
    else
        FilePirate::Application().ocean->sendMessage(NET_OCEAN_ADMIN_LOCK_RESP, AdminLock::lockKey.prepend(NET_OCEAN_NEGATIVE_ACK));
}

void AdminLock::handleLockRequestResponse(QHostAddress host, bool success, QString key)
{
    AdminLock::lockKey = key;
    if (success == false) {
        this->temporaryKey = "";
        AdminLock::clientHasLock = false;
    } else if (this->temporaryKey == key) {
        AdminLock::clientHasLock = true;
    }
}

void AdminLock::requestAdminLock(QString key)
{
    AdminLock::clientHasLock = false;
    this->temporaryKey = key;
    QTimer::singleShot(15000, this, SLOT(handleLockRequestTimeout()));
}

void AdminLock::handleLockRequestTimeout()
{
    if (this->temporaryKey == "")
        return; // we were previously invalidated
    // If we received no response, we're the first on the network so we assume admin
    AdminLock::lockKey = this->temporaryKey;
    AdminLock::clientHasLock = true;
}
