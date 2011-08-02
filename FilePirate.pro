#-------------------------------------------------
#
# Project created by QtCreator 2010-07-12T14:38:34
#
#-------------------------------------------------

QT       += core gui xml

DEFINES += FILEPIRATE_VERSION=0x000901

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
    src/filelist.cpp \
    src/localfilemonitor.cpp \
    src/settingswindow.cpp \
    src/aboutwindow.cpp

HEADERS  += \
    src/filepirate.h \
    src/mainwindow.h \
    regex/pcre.h \
    regex/internal.h \
    regex/config.h \
    src/filelist.h \
    src/localfilemonitor.h \
    src/settingswindow.h \
    src/aboutwindow.h

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
