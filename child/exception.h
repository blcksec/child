#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "child/node.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Exception,);

#define CHILD_EXCEPTION(ARGS...) ExceptionPointer(new Exception(Node::context()->child("Exception"), ##ARGS))

#define CHILD_THROW(EXCEPTION, MESSAGE) \
throw EXCEPTION##Pointer(new EXCEPTION(Node::context()->child(#EXCEPTION), MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO))

#define CHILD_TODO \
CHILD_THROW(Exception, "function not yet implemented")

class Exception : public Node {
    CHILD_DECLARATION(Exception, Node);
public:
    QString message;
    QString file;
    int line;
    QString function;

    Exception(const Pointer &origin, const QString &message = "", const QString &file = "",
              const int line = 0, const QString &function = "") :
        Node(origin), message(message), file(file), line(line), function(function) {}

    static void initRoot() { Node::root()->addChild("Exception", root()); }

    virtual Pointer fork() const { return new Exception(this, message, file, line, function); }

    const QString report() const;

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return report();
    }
};

CHILD_POINTER_DEFINITION(Exception,);

#define CHILD_EXCEPTION_DECLARATION(NAME, ORIGIN) \
CHILD_POINTER_DECLARATION(NAME, ORIGIN); \
class NAME : public ORIGIN { \
    CHILD_DECLARATION(NAME, ORIGIN); \
public: \
    NAME(const Pointer &origin, const QString &message = "", const QString &file = "", \
         const int line = 0, const QString &function = "") : \
        ORIGIN(origin, message, file, line, function) {} \
    static void initRoot() { Node::root()->addChild(#NAME, root()); } \
    virtual Pointer fork() const { \
        return new NAME(this, message, file, line, function); \
    } \
}; \
CHILD_POINTER_DEFINITION(NAME, ORIGIN);

#define CHILD_EXCEPTION_DEFINITION(NAME, ORIGIN) \
CHILD_DEFINITION(NAME, ORIGIN);

CHILD_EXCEPTION_DECLARATION(LexerException, Exception);
CHILD_EXCEPTION_DECLARATION(ParserException, Exception);
CHILD_EXCEPTION_DECLARATION(RuntimeException, Exception);
CHILD_EXCEPTION_DECLARATION(ArgumentException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NullPointerException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(IndexOutOfBoundsException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NotFoundException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(DuplicateException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(TypecastException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(ConversionException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(OperatingSystemException, Exception);
CHILD_EXCEPTION_DECLARATION(FileSystemException, OperatingSystemException);

CHILD_END

#endif // CHILD_EXCEPTION_H
