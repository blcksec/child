#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "child/node.h"

#define CHILD_THROW(EXCEPTION, MESSAGE) \
throw(EXCEPTION::make(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO));

#define CHILD_EXCEPTION_DECLARATION(NAME, ORIGIN, PARENT) \
CHILD_PTR_DECLARATION(NAME, ORIGIN, PARENT); \
class NAME : public ORIGIN { \
    CHILD_DECLARATION(NAME, ORIGIN, PARENT); \
public: \
    NAME(const QString &message = "", const QString &file = "", \
              const int line = 0, const QString &function = "") : \
        ORIGIN(message, file, line, function) {} \
    static NAME##Ptr make(const QString &message = "", const QString &file = "", \
                             const int line = 0, const QString &function = ""); \
}; \
CHILD_PTR_DEFINITION(NAME, ORIGIN, PARENT);

#define CHILD_EXCEPTION_DEFINITION(NAME, ORIGIN, PARENT) \
CHILD_DEFINITION(NAME, ORIGIN, PARENT); \
void NAME::initRoot() {} \
NAME##Ptr NAME::make(const QString &message, const QString &file, \
                             const int line, const QString &function) { \
    NAME##Ptr f(new NAME(message, file, line, function)); \
    f->setOrigin(context()->child(#NAME)); \
    return f; \
}

namespace Child {

CHILD_PTR_DECLARATION(Exception, Node, Node);

class Exception : public Node {
    CHILD_DECLARATION(Exception, Node, Node);
public:
    Exception(const QString &message = "", const QString &file = "",
              const int line = 0, const QString &function = "") :
        _message(message), _file(file), _line(line), _function(function) {}

    static ExceptionPtr make(const QString &message = "", const QString &file = "",
                             const int line = 0, const QString &function = "");

    const QString report() const;

    virtual const QString inspect() const { return report(); }
private:
    const QString _message;
    const QString _file;
    const int _line;
    const QString _function;
};

CHILD_PTR_DEFINITION(Exception, Node, Node);

CHILD_EXCEPTION_DECLARATION(LexerException, Exception, Node);

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
