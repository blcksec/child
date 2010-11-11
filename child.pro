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
    child/node.cpp \
    child/nativemethod.cpp \
    child/text.cpp \
    child/toolbox.cpp \
    child/object.cpp \
    child/world.cpp \
    child/dictionary.cpp \
    tests/nodetest.cpp \
    tests/texttest.cpp \
    tests/objecttest.cpp \
    tests/worldtest.cpp \
    tests/dictionarytest.cpp \
    child/lexer.cpp

HEADERS += \
    child/node.h \
    child/nativemethod.h \
    child/text.h \
    child/toolbox.h \
    child/exception.h \
    child/object.h \
    child/world.h \
    tests/nodetest.h \
    tests/objecttest.h \
    tests/runalltests.h \
    tests/testhelper.h \
    tests/objecttest.h \
    tests/worldtest.h \
    tests/texttest.h \
    child/dictionary.h \
    tests/dictionarytest.h \
    child/lexer.h

OTHER_FILES += \
    notes/ideas.txt \
    notes/syntax.txt \
    notes/pseudocode.txt \
    README.txt \
    notes/todo.txt \
    notes/old.txt
