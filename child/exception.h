#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QtCore/QString>

namespace Child {
    class Exception {
    public:
        Exception(const QString &msg = "Undefined Exception") : message(msg) { qDebug() << msg; }
        const QString message;
    };

    class LexerException : public Exception {
    public:
        LexerException(const QString &msg = "Undefined LexerException") : Exception(msg) {}
    };

    class ParserException : public Exception {
    public:
        ParserException(const QString &msg = "Undefined ParserException") : Exception(msg) {}
    };

    class RuntimeException : public Exception {
    public:
        RuntimeException(const QString &msg = "Undefined RuntimeException") : Exception(msg) {}
    };

    class ArgumentException : public RuntimeException {
    public:
        ArgumentException(const QString &msg = "Undefined ArgumentException") : RuntimeException(msg) {}
    };

    class NullPointerException : public RuntimeException {
    public:
        NullPointerException(const QString &msg = "Undefined NullPointerException") : RuntimeException(msg) {}
    };

    class IndexOutOfBoundsException : public RuntimeException {
    public:
        IndexOutOfBoundsException(const QString &msg = "Undefined IndexOutOfBoundsException") : RuntimeException(msg) {}
    };

    class NotFoundException : public RuntimeException {
    public:
        NotFoundException(const QString &msg = "Undefined NotFoundException") : RuntimeException(msg) {}
    };

    class DuplicateException : public RuntimeException {
    public:
        DuplicateException(const QString &msg = "Undefined DuplicateException") : RuntimeException(msg) {}
    };
}

#endif // EXCEPTION_H
