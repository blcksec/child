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
    }

    virtual Pointer fork() const { return new Object(this); }

    CHILD_NATIVE_METHOD_DECLARE(less_than);
    CHILD_NATIVE_METHOD_DECLARE(less_than_or_equal_to);
    CHILD_NATIVE_METHOD_DECLARE(greater_than);
    CHILD_NATIVE_METHOD_DECLARE(greater_than_or_equal_to);

    CHILD_NATIVE_METHOD_DECLARE(if);
};

CHILD_POINTER_DEFINE(Object,);

CHILD_END

#endif // CHILD_OBJECT_H
