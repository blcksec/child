#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"
#include "child/exception.h"
#include "child/nativemethod.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Object,);

#define CHILD_OBJECT(ARGS...) ObjectPointer(new Object(Node::context()->child("Object"), ##ARGS))

class Object : public Node {
    CHILD_DECLARE(Object, Node);
public:
    Object(const Pointer &origin) : Node(origin) {}

    static void initRoot() {
        Node::root()->addChild("Object", root());
        CHILD_NATIVE_METHOD_ADD(Object, less_than, <);
        CHILD_NATIVE_METHOD_ADD(Object, less_than_or_equal_to, <=);
        CHILD_NATIVE_METHOD_ADD(Object, greater_than, >);
        CHILD_NATIVE_METHOD_ADD(Object, greater_than_or_equal_to, >=);

        CHILD_NATIVE_METHOD_ADD(Object, if);
        CHILD_NATIVE_METHOD_ADD(Object, unless);
        CHILD_NATIVE_METHOD_ADD(Object, loop);
        CHILD_NATIVE_METHOD_ADD(Object, while);
        CHILD_NATIVE_METHOD_ADD(Object, until);
        CHILD_NATIVE_METHOD_ADD(Object, break);
    }

    virtual Pointer fork() const { return new Object(this); }

    CHILD_NATIVE_METHOD_DECLARE(less_than);
    CHILD_NATIVE_METHOD_DECLARE(less_than_or_equal_to);
    CHILD_NATIVE_METHOD_DECLARE(greater_than);
    CHILD_NATIVE_METHOD_DECLARE(greater_than_or_equal_to);
private:
    Pointer ifOrUnless(const MessagePointer &message, bool isIf);
public:
    CHILD_NATIVE_METHOD_DECLARE(if) { return(ifOrUnless(message, true)); }
    CHILD_NATIVE_METHOD_DECLARE(unless) { return(ifOrUnless(message, false)); }

    CHILD_NATIVE_METHOD_DECLARE(loop);
private:
    Pointer whileOrUntil(const MessagePointer &message, bool isWhile);
public:
    CHILD_NATIVE_METHOD_DECLARE(while) { return(whileOrUntil(message, true)); }
    CHILD_NATIVE_METHOD_DECLARE(until) { return(whileOrUntil(message, false)); }

    class Break {
    public:
        Pointer result;
        Break(Pointer const &result = Pointer::null()) : result(result) {}
    };

    CHILD_NATIVE_METHOD_DECLARE(break);
};

CHILD_POINTER_DEFINE(Object,);

CHILD_END

#endif // CHILD_OBJECT_H
