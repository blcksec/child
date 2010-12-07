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
    tests/nodetest.cpp \
    child/toolbox.cpp \
    child/exception.cpp \
#    child/nativemethod.cpp \
    child/object.cpp \
    tests/objecttest.cpp \
    child/element.cpp \
    child/boolean.cpp \
    child/number.cpp \
    child/character.cpp \
    child/text.cpp \
    tests/texttest.cpp \
    child/pair.cpp \
    child/list.cpp \
    tests/listtest.cpp \
    child/dictionary.cpp \
    tests/dictionarytest.cpp \
    child/application.cpp \
#    child/message.cpp \
    child/language.cpp \
    child/language/token.cpp \
    child/language/operator.cpp \
    child/language/operatortable.cpp \
    child/language/primitive.cpp \
    child/language/primitivechain.cpp \
    child/language/section.cpp \
    child/language/block.cpp \
    child/language/sourcecode.cpp \
    child/language/argument.cpp
#    child/language/lexer.cpp \
#    child/language/parser.cpp \
#    child/world.cpp \
#    tests/worldtest.cpp

HEADERS += \
    child.h \
    child/node.h \
    tests/nodetest.h \
    child/toolbox.h \
    child/exception.h \
    tests/testhelper.h \
    tests/runalltests.h \
#    child/nativemethod.h \
    child/object.h \
    tests/objecttest.h \
    child/element.h \
    child/boolean.h \
    child/number.h \
    child/character.h \
    child/text.h \
    tests/texttest.h \
    child/pair.h \
    child/list.h \
    tests/listtest.h \
    child/dictionary.h \
    tests/dictionarytest.h \
    child/application.h \
#    child/message.h \
    child/language.h \
    child/language/token.h \
    child/language/operator.h \
    child/language/operatortable.h \
    child/language/primitive.h \
    child/language/primitivechain.h \
    child/language/section.h \
    child/language/block.h \
    child/language/sourcecode.h \
    child/language/argument.h
#    child/language/lexer.h \
#    child/language/parser.h \
#    child/world.h \
#    tests/worldtest.h

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
