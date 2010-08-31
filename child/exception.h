#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QtCore/QString>

namespace Child {
    class Exception {
    public:
        Exception(const QString &msg = "Undefined Exception") : message(msg) {}
        const QString message;
    };

    class ArgumentException : public Exception {
    public:
        ArgumentException(const QString &msg = "Undefined ArgumentException") : Exception(msg) {}
    };

    class NullPointerException : public Exception {
    public:
        NullPointerException(const QString &msg = "Undefined NullPointerException") : Exception(msg) {}
    };

    class IndexOutOfBoundsException : public Exception {
    public:
        IndexOutOfBoundsException(const QString &msg = "Undefined IndexOutOfBoundsException") : Exception(msg) {}
    };

    class NotFoundException : public Exception {
    public:
        NotFoundException(const QString &msg = "Undefined NotFoundException") : Exception(msg) {}
    };

    class DuplicateException : public Exception {
    public:
        DuplicateException(const QString &msg = "Undefined DuplicateException") : Exception(msg) {}
    };
}

#endif // EXCEPTION_H
