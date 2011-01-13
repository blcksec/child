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
    node/exception.cpp \
    tests/nodetest.cpp \
    node/nativemethod.cpp \
    node/object.cpp \
    tests/objecttest.cpp \
    node/element.cpp \
    node/boolean.cpp \
    node/number.cpp \
    node/character.cpp \
    node/text.cpp \
    tests/texttest.cpp \
    node/pair.cpp \
    node/list.cpp \
    tests/listtest.cpp \
    node/bunch.cpp \
    node/dictionary.cpp \
    tests/dictionarytest.cpp \
    node/block.cpp \
    node/method.cpp \
    node/message.cpp \
    node/controlflow.cpp \
    node/application.cpp \
    node/language.cpp \
    node/language/token.cpp \
    node/language/operator.cpp \
    node/language/operatortable.cpp \
    node/language/primitive.cpp \
    node/language/argument.cpp \
    node/language/argumentbunch.cpp \
    node/language/parameter.cpp \
    node/language/parameterlist.cpp \
    node/language/section.cpp \
    node/language/sourcecode.cpp \
    node/language/sourcecodedictionary.cpp \
    node/language/test.cpp \
    node/language/testsuite.cpp \
    node/language/lexer.cpp \
    node/language/parser.cpp \
    node/language/interpreter.cpp \
    node/property.cpp \
    node.cpp \
    global.cpp

HEADERS += \
    node/exception.h \
    tests/testbegin.h \
    tests/testend.h \
    tests/runalltests.h \
    tests/nodetest.h \
    node/nativemethod.h \
    node/object.h \
    tests/objecttest.h \
    node/element.h \
    node/boolean.h \
    node/number.h \
    node/character.h \
    node/text.h \
    tests/texttest.h \
    node/pair.h \
    node/list.h \
    tests/listtest.h \
    node/bunch.h \
    node/dictionary.h \
    tests/dictionarytest.h \
    node/block.h \
    node/method.h \
    node/message.h \
    node/controlflow.h \
    node/application.h \
    node/language.h \
    node/language/token.h \
    node/language/operator.h \
    node/language/operatortable.h \
    node/language/primitive.h \
    node/language/argument.h \
    node/language/argumentbunch.h \
    node/language/parameter.h \
    node/language/parameterlist.h \
    node/language/section.h \
    node/language/sourcecode.h \
    node/language/sourcecodedictionary.h \
    node/language/test.h \
    node/language/testsuite.h \
    node/language/lexer.h \
    node/language/parser.h \
    node/language/interpreter.h \
    node/property.h \
    node.h \
    global.h

OTHER_FILES += \
    notes/ideas.txt \
    notes/syntax.txt \
    notes/pseudocode.txt \
    README.txt \
    notes/todo.txt \
    notes/old.txt \
    examples/lexertest.child \
    notes/specification.txt \
    examples/test.child \
    notes/features.txt \
    node/node.child \
    node/exception.child \
    node/object.child
