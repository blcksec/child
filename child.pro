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
    child.cpp \
    child/node.cpp \
    child/toolbox.cpp \
    tests/nodetest.cpp \
    child/exception.cpp
#    child/nativemethod.cpp \
#    child/text.cpp \
#    child/object.cpp \
#    child/world.cpp \
#    child/dictionary.cpp \
#    tests/texttest.cpp \
#    tests/objecttest.cpp \
#    tests/worldtest.cpp \
#    tests/dictionarytest.cpp \
#    child/application.cpp \
#    child/block.cpp \
#    child/message.cpp \
#    child/list.cpp \
#    tests/listtest.cpp \
#    child/number.cpp \
#    child/boolean.cpp \
#    child/character.cpp \
#    child/language/lexer.cpp \
#    child/language/token.cpp \
#    child/language/sourcecode.cpp \
#    child/language/section.cpp \
#    child/language/primitivechain.cpp \
#    child/language/primitive.cpp \
#    child/language/parser.cpp \
#    child/language/operatortable.cpp \
#    child/language/operator.cpp \
#    child/language.cpp

HEADERS += \
    child.h \
    child/node.h \
    child/exception.h \
    child/toolbox.h \
    tests/testhelper.h \
    tests/nodetest.h \
    tests/runalltests.h
#    child/nativemethod.h \
#    child/text.h \
#    child/object.h \
#    child/world.h \
#    tests/objecttest.h \
#    tests/worldtest.h \
#    tests/texttest.h \
#    child/dictionary.h \
#    tests/dictionarytest.h \
#    child/application.h \
#    child/block.h \
#    child/message.h \
#    child/list.h \
#    tests/listtest.h \
#    child/number.h \
#    child/boolean.h \
#    child/character.h \
#    child/language.h \
#    child/language/lexer.h \
#    child/language/token.h \
#    child/language/sourcecode.h \
#    child/language/section.h \
#    child/language/primitivechain.h \
#    child/language/primitive.h \
#    child/language/parser.h \
#    child/language/operatortable.h \
#    child/language/operator.h

OTHER_FILES += \
    notes/ideas.txt \
    notes/syntax.txt \
    notes/pseudocode.txt \
    README.txt \
    notes/todo.txt \
    notes/old.txt \
    examples/lexertest.child \
    notes/specification.txt \
    examples/test.child
