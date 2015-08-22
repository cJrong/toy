TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    options.cpp \
    utf8.cpp \
    dump_class_file.cpp \
    jar.cpp \
    jvm.cpp \
    opcode.cpp \
    thread.cpp \
    zlib/adler32.c \
    zlib/compress.c \
    zlib/crc32.c \
    zlib/deflate.c \
    zlib/gzclose.c \
    zlib/gzlib.c \
    zlib/gzread.c \
    zlib/gzwrite.c \
    zlib/infback.c \
    zlib/inffast.c \
    zlib/inflate.c \
    zlib/inftrees.c \
    zlib/trees.c \
    zlib/uncompr.c \
    zlib/zutil.c \
    class_file.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    any.h \
    config.h \
    exception.h \
    reader.h \
    options.h \
    utf8.h \
    zlib/crc32.h \
    zlib/deflate.h \
    zlib/gzguts.h \
    zlib/inffast.h \
    zlib/inffixed.h \
    zlib/inflate.h \
    zlib/inftrees.h \
    zlib/trees.h \
    zlib/zconf.h \
    zlib/zlib.h \
    zlib/zutil.h \
    class.h \
    jar.h \
    jvm.h \
    opcode.h \
    stack_frame.h \
    thread.h \
    class_file.h

unix:!macx: QMAKE_CFLAGS +=
unix:!macx: QMAKE_CXXFLAGS += -std=c++11
