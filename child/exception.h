#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include <QtCore/QString>

namespace Child {
    class Exception {
    public:
        Exception(const QString &msg = "???") : _message(msg) {}
        virtual const QString message() const { return("Exception: " + _message); }
    protected:
        const QString _message;
    };

    class LexerException : public Exception {
    public:
        LexerException(const QString &msg = "???") : Exception(msg) {}
        virtual const QString message() const { return("LexerException: " + _message); }
    };

    class ParserException : public Exception {
    public:
        ParserException(const QString &msg = "???") : Exception(msg) {}
        virtual const QString message() const { return("ParserException: " + _message); }
    };

    class RuntimeException : public Exception {
    public:
        RuntimeException(const QString &msg = "???") : Exception(msg) {}
        virtual const QString message() const { return("RuntimeException: " + _message); }
    };

    class ArgumentException : public RuntimeException {
    public:
        ArgumentException(const QString &msg = "???") : RuntimeException(msg) {}
        virtual const QString message() const { return("ArgumentException: " + _message); }
    };

    class NullPointerException : public RuntimeException {
    public:
        NullPointerException(const QString &msg = "???") : RuntimeException(msg) {}
        virtual const QString message() const { return("NullPointerException: " + _message); }
    };

    class IndexOutOfBoundsException : public RuntimeException {
    public:
        IndexOutOfBoundsException(const QString &msg = "???") : RuntimeException(msg) {}
        virtual const QString message() const { return("IndexOutOfBoundsException: " + _message); }
    };

    class NotFoundException : public RuntimeException {
    public:
        NotFoundException(const QString &msg = "???") : RuntimeException(msg) {}
        virtual const QString message() const { return("NotFoundException: " + _message); }
    };

    class DuplicateException : public RuntimeException {
    public:
        DuplicateException(const QString &msg = "???") : RuntimeException(msg) {}
        virtual const QString message() const { return("DuplicateException: " + _message); }
    };

    class OperatingSystemException : public Exception {
    public:
        OperatingSystemException(const QString &msg = "???") : Exception(msg) {}
        virtual const QString message() const { return("OperatingSystemException: " + _message); }
    };

    class FileSystemException : public OperatingSystemException {
    public:
        FileSystemException(const QString &msg = "???") : OperatingSystemException(msg) {}
        virtual const QString message() const { return("FileSystemException: " + _message); }
    };
}

#endif // CHILD_EXCEPTION_H
