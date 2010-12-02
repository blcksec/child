#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "child/node.h"

#define CHILD_THROW(EXCEPTION, MESSAGE) \
throw(EXCEPTION##Ptr(new EXCEPTION(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)));

#define CHILD_TODO \
CHILD_THROW(Exception, "function not yet implemented");

#define CHILD_EXCEPTION_DECLARATION(NAME, ORIGIN, PARENT) \
CHILD_PTR_DECLARATION(NAME, ORIGIN, PARENT); \
class NAME : public ORIGIN { \
    CHILD_DECLARATION(NAME, ORIGIN, PARENT); \
public: \
    NAME(const ORIGIN##Ptr &origin) : ORIGIN(origin) {} \
    NAME(const QString &message = "", const QString &file = "", \
         const int line = 0, const QString &function = "", \
         const ORIGIN##Ptr &origin = find(#NAME)) : \
        ORIGIN(message, file, line, function, origin) {} \
}; \
CHILD_PTR_DEFINITION(NAME, ORIGIN, PARENT);

#define CHILD_EXCEPTION_DEFINITION(NAME, ORIGIN, PARENT) \
CHILD_DEFINITION(NAME, ORIGIN, PARENT); \
void NAME::initRoot() { \
    PARENT::root()->addChild(#NAME, root()); \
}

namespace Child {

CHILD_PTR_DECLARATION(Exception, Node, Node);

class Exception : public Node {
    CHILD_DECLARATION(Exception, Node, Node);
public:
    QString message;
    QString file;
    int line;
    QString function;

    Exception(const NodePtr &origin) : Node(origin) {}

    Exception(const QString &message = "", const QString &file = "",
              const int line = 0, const QString &function = "",
              const NodePtr &origin = find("Exception")) :
        Node(origin), message(message), file(file), line(line), function(function) {}

    const QString report() const;

    virtual const QString inspect() const { return report(); }
};

CHILD_PTR_DEFINITION(Exception, Node, Node);

CHILD_EXCEPTION_DECLARATION(LexerException, Exception, Node);
CHILD_EXCEPTION_DECLARATION(ParserException, Exception, Node);
CHILD_EXCEPTION_DECLARATION(RuntimeException, Exception, Node);
CHILD_EXCEPTION_DECLARATION(ArgumentException, RuntimeException, Node);
CHILD_EXCEPTION_DECLARATION(NullPointerException, RuntimeException, Node);
CHILD_EXCEPTION_DECLARATION(IndexOutOfBoundsException, RuntimeException, Node);
CHILD_EXCEPTION_DECLARATION(NotFoundException, RuntimeException, Node);
CHILD_EXCEPTION_DECLARATION(DuplicateException, RuntimeException, Node);
CHILD_EXCEPTION_DECLARATION(OperatingSystemException, Exception, Node);
CHILD_EXCEPTION_DECLARATION(FileSystemException, OperatingSystemException, Node);

} // namespace Child

#endif // CHILD_EXCEPTION_H
