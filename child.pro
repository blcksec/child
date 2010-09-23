#-------------------------------------------------
#
# Project created by QtCreator 2010-08-23T15:21:38
#
#-------------------------------------------------

QT       += core
QT       += testlib
QT       -= gui

TARGET = child
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    child/module.cpp \
    child/nativemethod.cpp \
    child/text.cpp \
    child/toolbox.cpp \
    child/object.cpp \
    child/world.cpp \
    child/dictionary.cpp

HEADERS += \
    child/module.h \
    child/nativemethod.h \
    child/text.h \
    child/toolbox.h \
    child/exception.h \
    child/object.h \
    child/world.h \
    tests/objecttest.h \
    tests/runalltests.h \
    tests/moduletest.h \
    tests/testhelper.h \
    tests/objecttest.h \
    tests/worldtest.h \
    tests/texttest.h \
    child/dictionary.h

OTHER_FILES += \
    notes/ideas.txt \
    notes/syntax.txt \
    notes/pseudocode.txt \
    README.txt
