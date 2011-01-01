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

    static void initRoot() {
        Node::root()->addChild("Object", root());

        root()->addExtension(ControlFlow::root());

        CHILD_NATIVE_METHOD_ADD(Object, postfix_increment, postfix++);
        CHILD_NATIVE_METHOD_ADD(Object, postfix_decrement, postfix--);

        CHILD_NATIVE_METHOD_ADD(Object, add_assign, +=);
        CHILD_NATIVE_METHOD_ADD(Object, subtract_assign, -=);
        CHILD_NATIVE_METHOD_ADD(Object, multiply_assign, *=);
        CHILD_NATIVE_METHOD_ADD(Object, divide_assign, /=);
        CHILD_NATIVE_METHOD_ADD(Object, modulo_assign, %=);

        CHILD_NATIVE_METHOD_ADD(Object, less_than, <);
        CHILD_NATIVE_METHOD_ADD(Object, less_than_or_equal_to, <=);
        CHILD_NATIVE_METHOD_ADD(Object, greater_than, >);
        CHILD_NATIVE_METHOD_ADD(Object, greater_than_or_equal_to, >=);

        CHILD_NATIVE_METHOD_ADD(Object, while);
        CHILD_NATIVE_METHOD_ADD(Object, until);
        CHILD_NATIVE_METHOD_ADD(Object, return);
    }

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
private:
    Node *whileOrUntil(Message *message, bool isWhile);
public:
    CHILD_NATIVE_METHOD_DECLARE(while) { return whileOrUntil(message, true); }
    CHILD_NATIVE_METHOD_DECLARE(until) { return whileOrUntil(message, false); }

    class Break {
    public:
        Node *result;
        Break(Node *result = NULL) : result(result) {}
    };

    class Return {
    public:
        Node *result;
        Return(Node *result = NULL) : result(result) {}
    };

    CHILD_NATIVE_METHOD_DECLARE(return);
};

CHILD_END

#endif // CHILD_OBJECT_H
