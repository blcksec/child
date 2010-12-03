#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "child/node.h"

#define CHILD_THROW(EXCEPTION, MESSAGE) \
throw(EXCEPTION##Ptr(new EXCEPTION(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)));

#define CHILD_TODO \
CHILD_THROW(Exception, "function not yet implemented");

#define CHILD_EXCEPTION_DECLARATION(NAME, ORIGIN) \
CHILD_PTR_DECLARATION(NAME, ORIGIN); \
class NAME : public ORIGIN { \
    CHILD_DECLARATION(NAME, ORIGIN); \
public: \
    NAME(const ORIGIN##Ptr &origin) : ORIGIN(origin) {} \
    NAME(const QString &message = "", const QString &file = "", \
         const int line = 0, const QString &function = "") : \
        ORIGIN(message, file, line, function) {} \
}; \
CHILD_PTR_DEFINITION(NAME, ORIGIN);

#define CHILD_EXCEPTION_DEFINITION(NAME, ORIGIN) \
CHILD_DEFINITION(NAME, ORIGIN); \
bool NAME::initRoot() { \
    Node::root()->addChild(#NAME, root()); \
    return true; \
}

namespace Child {

CHILD_PTR_DECLARATION(Exception, Node);

class Exception : public Node {
    CHILD_DECLARATION(Exception, Node);
public:
    QString message;
    QString file;
    int line;
    QString function;

    Exception(const NodePtr &origin) : Node(origin) {} // root constructor

    Exception(const ExceptionPtr &origin) : Node(origin), message(origin->message), // fork constructor
        file(origin->file), line(origin->line), function(origin->function) {}

    Exception(const QString &message = "", const QString &file = "",  // convenience constructor
              const int line = 0, const QString &function = "") :
        Node(find("Exception")), message(message), file(file), line(line), function(function) {}

    const QString report() const;

    virtual const QString inspect() const { return report(); }
};

CHILD_PTR_DEFINITION(Exception, Node);

CHILD_EXCEPTION_DECLARATION(LexerException, Exception);
CHILD_EXCEPTION_DECLARATION(ParserException, Exception);
CHILD_EXCEPTION_DECLARATION(RuntimeException, Exception);
CHILD_EXCEPTION_DECLARATION(ArgumentException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NullPointerException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(IndexOutOfBoundsException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NotFoundException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(DuplicateException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(OperatingSystemException, Exception);
CHILD_EXCEPTION_DECLARATION(FileSystemException, OperatingSystemException);

} // namespace Child

#endif // CHILD_EXCEPTION_H
