#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "child/node.h"

namespace Child {

class ExceptionPtr;

class Exception : public Node {
public:
    Exception() {}

    static ExceptionPtr &root();
    static void initRoot();

    static ExceptionPtr make();

    virtual NodePtr fork() const {
        NodePtr f(new Exception);
        f->setOrigin(this);
        f->initFork();
        return f;
    }
private:
    static ExceptionPtr _root;
};

class ExceptionPtr : public NodePtr {
public:
    ExceptionPtr() {}
    ExceptionPtr(const Exception *data) : NodePtr(data) {}
    ExceptionPtr(const NodePtr &other) : NodePtr(other) {}

    Exception &operator*() { return *static_cast<Exception *>(NodePtr::data()); };
    const Exception &operator*() const { return *static_cast<const Exception *>(NodePtr::data()); };
    Exception *operator->() { return static_cast<Exception *>(NodePtr::data()); };
    const Exception *operator->() const { return static_cast<const Exception *>(NodePtr::data()); };
    const Exception *setData(const Exception *data) { return static_cast<const Exception *>(NodePtr::setData(data)); }
    ExceptionPtr& operator=(const ExceptionPtr &other) { NodePtr::setData(other._data); return *this; }
};

//    class Exception {
//    public:
//        Exception(const QString &msg = "???") : _message(msg) {}
//        virtual const QString message() const { return "Exception: " + _message; }
//    protected:
//        const QString _message;
//    };

//    class LexerException : public Exception {
//    public:
//        LexerException(const QString &msg = "???") : Exception(msg) {}
//        virtual const QString message() const { return "LexerException: " + _message; }
//    };

//    class ParserException : public Exception {
//    public:
//        ParserException(const QString &msg = "???") : Exception(msg) {}
//        virtual const QString message() const { return "ParserException: " + _message; }
//    };

//    class RuntimeException : public Exception {
//    public:
//        RuntimeException(const QString &msg = "???") : Exception(msg) {}
//        virtual const QString message() const { return "RuntimeException: " + _message; }
//    };

//    class ArgumentException : public RuntimeException {
//    public:
//        ArgumentException(const QString &msg = "???") : RuntimeException(msg) {}
//        virtual const QString message() const { return "ArgumentException: " + _message; }
//    };

//    class NullPointerException : public RuntimeException {
//    public:
//        NullPointerException(const QString &msg = "???") : RuntimeException(msg) {}
//        virtual const QString message() const { return "NullPointerException: " + _message; }
//    };

//    class IndexOutOfBoundsException : public RuntimeException {
//    public:
//        IndexOutOfBoundsException(const QString &msg = "???") : RuntimeException(msg) {}
//        virtual const QString message() const { return "IndexOutOfBoundsException: " + _message; }
//    };

//    class NotFoundException : public RuntimeException {
//    public:
//        NotFoundException(const QString &msg = "???") : RuntimeException(msg) {}
//        virtual const QString message() const { return "NotFoundException: " + _message; }
//    };

//    class DuplicateException : public RuntimeException {
//    public:
//        DuplicateException(const QString &msg = "???") : RuntimeException(msg) {}
//        virtual const QString message() const { return "DuplicateException: " + _message; }
//    };

//    class OperatingSystemException : public Exception {
//    public:
//        OperatingSystemException(const QString &msg = "???") : Exception(msg) {}
//        virtual const QString message() const { return "OperatingSystemException: " + _message; }
//    };

//    class FileSystemException : public OperatingSystemException {
//    public:
//        FileSystemException(const QString &msg = "???") : OperatingSystemException(msg) {}
//        virtual const QString message() const { return "FileSystemException: " + _message; }
//    };

} // namespace Child

#endif // CHILD_EXCEPTION_H
