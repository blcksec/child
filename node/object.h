#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"
#include "child/exception.h"
#include "child/nativemethod.h"
#include "child/controlflow.h"

CHILD_BEGIN

#define CHILD_OBJECT(ARGS...) new Object(Node::context()->child("Object"), ##ARGS)

class Object : public Node {
    CHILD_DECLARE(Object, Node);
public:
    explicit Object(Node *origin) : Node(origin) {}

    CHILD_FORK_METHOD(Object);

    CHILD_NATIVE_METHOD_DECLARE(postfix_increment);
    CHILD_NATIVE_METHOD_DECLARE(postfix_decrement);

    CHILD_NATIVE_METHOD_DECLARE(add_assign);
    CHILD_NATIVE_METHOD_DECLARE(subtract_assign);
    CHILD_NATIVE_METHOD_DECLARE(multiply_assign);
    CHILD_NATIVE_METHOD_DECLARE(divide_assign);
    CHILD_NATIVE_METHOD_DECLARE(modulo_assign);

    CHILD_NATIVE_METHOD_DECLARE(less_than);
    CHILD_NATIVE_METHOD_DECLARE(less_than_or_equal_to);
    CHILD_NATIVE_METHOD_DECLARE(greater_than);
    CHILD_NATIVE_METHOD_DECLARE(greater_than_or_equal_to);
public:
    class Return {
    public:
        Node *result;
        Return(Node *result = NULL) : result(result) {}
    };

    CHILD_NATIVE_METHOD_DECLARE(return);
};

CHILD_END

#endif // CHILD_OBJECT_H
