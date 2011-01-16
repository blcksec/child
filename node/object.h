#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "node.h"
#include "node/exception.h"
#include "node/nativemethod.h"
#include "node/controlflow.h"

CHILD_BEGIN

#define CHILD_OBJECT(ARGS...) new Object(context()->child("Object"), ##ARGS)

class Object : public Node {
    CHILD_DECLARE(Object, Node, Node);
public:
    explicit Object(Node *origin) : Node(origin) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Object);

    CHILD_DECLARE_NATIVE_METHOD(postfix_increment);
    CHILD_DECLARE_NATIVE_METHOD(postfix_decrement);

    CHILD_DECLARE_NATIVE_METHOD(add_assign);
    CHILD_DECLARE_NATIVE_METHOD(subtract_assign);
    CHILD_DECLARE_NATIVE_METHOD(multiply_assign);
    CHILD_DECLARE_NATIVE_METHOD(divide_assign);
    CHILD_DECLARE_NATIVE_METHOD(modulo_assign);

    CHILD_DECLARE_NATIVE_METHOD(less_than);
    CHILD_DECLARE_NATIVE_METHOD(less_than_or_equal_to);
    CHILD_DECLARE_NATIVE_METHOD(greater_than);
    CHILD_DECLARE_NATIVE_METHOD(greater_than_or_equal_to);
public:
    class Return {
    public:
        Node *result;
        Return(Node *result = NULL) : result(result) {}
    };

    CHILD_DECLARE_NATIVE_METHOD(return);
};

CHILD_END

#endif // CHILD_OBJECT_H
