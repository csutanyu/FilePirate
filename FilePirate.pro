#-------------------------------------------------
#
# Project created by QtCreator 2010-07-12T14:38:34
#
#-------------------------------------------------

QT       += core gui xml sql network

DEFINES += FILEPIRATE_VERSION=0x000901 _CRT_SECURE_NO_WARNINGS=1

LIBS += -lQtNetwork

TARGET = FilePirate
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/filepirate.cpp \
    src/mainwindow.cpp \
    regex/study.c \
    regex/pcre.c \
    regex/get.c \
    regex/chartables.c \
    src/localfilemonitor.cpp \
    src/settingswindow.cpp \
    src/aboutwindow.cpp \
    src/sleepthread.cpp \
    src/file.cpp \
    src/localfilelist.cpp \
    src/filelist.cpp \
    src/net/oceansocket.cpp \
    src/net/cannonfactory.cpp \
    src/net/receiverfactory.cpp \
    src/net/barrelexchange.cpp \
    src/hash/murmurhash.cpp \
    src/hash/filehasher.cpp \
    src/admin/adminlock.cpp \
    zlib/zutil.c \
    zlib/uncompr.c \
    zlib/trees.c \
    zlib/inftrees.c \
    zlib/inflate.c \
    zlib/inffast.c \
    zlib/infback.c \
    zlib/gzio.c \
    zlib/deflate.c \
    zlib/crc32.c \
    zlib/compress.c \
    zlib/adler32.c \
    src/threads/receiver.cpp \
    src/threads/cannon.cpp

HEADERS  += \
    src/filepirate.h \
    src/mainwindow.h \
    regex/pcre.h \
    regex/internal.h \
    regex/config.h \
    src/localfilemonitor.h \
    src/settingswindow.h \
    src/aboutwindow.h \
    src/sleepthread.h \
    src/file.h \
    src/defines.h \
    src/localfilelist.h \
    src/filelist.h \
    src/net/oceansocket.h \
    src/net/cannonfactory.h \
    src/net/receiverfactory.h \
    src/net/barrelexchange.h \
    src/hash/murmurhash.h \
    src/hash/filehasher.h \
    src/admin/adminlock.h \
    zlib/zutil.h \
    zlib/zlib.h \
    zlib/zconf.in.h \
    zlib/zconf.h \
    zlib/trees.h \
    zlib/inftrees.h \
    zlib/inflate.h \
    zlib/inffixed.h \
    zlib/inffast.h \
    zlib/deflate.h \
    zlib/crc32.h \
    src/threads/receiver.h \
    src/threads/cannon.h

FORMS    += \
    src/mainwindow.ui \
    src/settingswindow.ui \
    src/aboutwindow.ui

RESOURCES += \
    resource.qrc

RC_FILE += filepirate.rc

OTHER_FILES += \
    filepirate.rc \
    filepirate.ico







