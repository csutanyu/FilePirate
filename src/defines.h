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

#ifndef DEFINES_H
#define DEFINES_H

#if WIN32
// WINDOWS

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef unsigned __int64 uint64_t;

#define DIRECTORY_SEPARATOR "\\"

#elif LINUX

#include <stdint.h>

#define DIRECTORY_SEPARATOR "/"

#endif

// Environment variables
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Network message prefixes and separators

// -> ocean
#define NET_OCEAN_HEARTBEAT         0x21
#define NET_OCEAN_COURSE_CHANGE     0x1
#define NET_OCEAN_SAIL              0xF
#define NET_OCEAN_RETURN_TO_PORT    0xE
#define NET_OCEAN_MESSAGE           0x7
#define NET_OCEAN_MESSAGE_SEPARATOR 0x1C
#define NET_OCEAN_SEARCH            0x5
#define NET_OCEAN_ACKNOWLEDGE       0x6
#define NET_OCEAN_NEGATIVE_ACK      0x15
#define NET_OCEAN_ADMIN_LOCK_REQ    0x10
#define NET_OCEAN_ADMIN_LOCK_RESP   0x11

// -> cannon
#define NET_CANNON_LOAD             0x5
#define NET_CANNON_ACKNOWLEDGE      0x6
#define NET_CANNON_FIRE             0x2
#define NET_CANNON_PACKET_HEADER    0x1
#define NET_CANNON_PACKET_SEPARATOR 0x1C
#define NET_CANNON_CANCEL           0x18

#define FILE_HASHING_CHUNK_SIZE     131072

#endif // DEFINES_H
