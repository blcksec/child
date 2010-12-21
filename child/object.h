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
    explicit Object(const Pointer &origin) : Node(origin) {}

    static void initRoot() {
        Node::root()->addChild("Object", root());
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

        CHILD_NATIVE_METHOD_ADD(Object, if);
        CHILD_NATIVE_METHOD_ADD(Object, unless);
        CHILD_NATIVE_METHOD_ADD(Object, loop);
        CHILD_NATIVE_METHOD_ADD(Object, while);
        CHILD_NATIVE_METHOD_ADD(Object, until);
        CHILD_NATIVE_METHOD_ADD(Object, break);
        CHILD_NATIVE_METHOD_ADD(Object, return);
    }

    virtual Pointer fork() const { return new Object(this); }

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

    class Return {
    public:
        Pointer result;
        Return(Pointer const &result = Pointer::null()) : result(result) {}
    };

    CHILD_NATIVE_METHOD_DECLARE(break);
    CHILD_NATIVE_METHOD_DECLARE(return);
};

CHILD_POINTER_DEFINE(Object,);

CHILD_END

#endif // CHILD_OBJECT_H
