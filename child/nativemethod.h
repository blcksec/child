#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

class Message;

#define CHILD_NATIVE_METHOD(ARGS...) new NativeMethod(Node::context()->child("NativeMethod"), ##ARGS)

#define CHILD_NATIVE_METHOD_ADD(CLASS, METHOD, NAME...) \
CLASS::root()->addOrSetChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_)))

typedef Node *(Node::*_MethodPointer_)(Message *);

class NativeMethod : public Node {
    CHILD_DECLARE(NativeMethod, Node);
public:
    explicit NativeMethod(const Node *origin, const _MethodPointer_ &method = NULL) : Node(origin), _method(method) {}
    static void initRoot() { Node::root()->addChild("NativeMethod", root()); }
    virtual NativeMethod *fork() const { return new NativeMethod(this, _method); }

    _MethodPointer_ method() const { return _method; }
    void setMethod(const _MethodPointer_ &method) { _method = method; }

    virtual Node *run(Node *receiver, Message *message) {
        return (receiver->*method())(message);
    }
private:
    _MethodPointer_ _method;
};

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
