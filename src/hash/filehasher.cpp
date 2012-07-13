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

#include "filehasher.h"
#include "murmurhash.h"
#include "../defines.h"
#include <QFile>
#include <QTextStream>

QByteArray FileHasher::hashFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    uint32_t hash[4];
    uint32_t seed = 42;
    QByteArray chunk;
    QByteArray hashValue = QByteArray();
    while (!file.atEnd()) {
        chunk = file.read(FILE_HASHING_CHUNK_SIZE);
        chunk.prepend(hashValue);
#ifdef ENVIRONMENT32
        MurmurHash3_x86_128(chunk.data(), chunk.length(), seed, hash);
#else
        MurmurHash3_x64_128(chunk.data(), chunk.length(), seed, hash);
#endif
        // set our hash to be prepended to the next chunk
        hashValue = QByteArray::fromRawData(reinterpret_cast<const char*>(hash),4*sizeof(uint32_t));
    }
    return hashValue;
}

FileHasher::FileHasher(QObject *parent) :
    QObject(parent)
{}
