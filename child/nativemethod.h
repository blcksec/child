#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

class MessagePointer;

CHILD_POINTER_DECLARE(NativeMethod,);

#define CHILD_NATIVE_METHOD(ARGS...) NativeMethodPointer(new NativeMethod(Node::context()->child("NativeMethod"), ##ARGS))

#define CHILD_NATIVE_METHOD_ADD(CLASS, METHOD, NAME...) \
CLASS::root()->addChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_)))

#define CHILD_NATIVE_METHOD_SET(CLASS, METHOD, NAME...) \
CLASS::root()->setChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_)))

typedef Pointer (Node::*_MethodPointer_)(const MessagePointer &);

class NativeMethod : public Node {
    CHILD_DECLARE(NativeMethod, Node);
public:
    NativeMethod(const Pointer &origin, const _MethodPointer_ &method = NULL) : Node(origin), _method(method) {}
    static void initRoot() { Node::root()->addChild("NativeMethod", root()); }
    virtual Pointer fork() const { return new NativeMethod(this, _method); }

    _MethodPointer_ method() const { return _method; }
    NativeMethodPointer setMethod(const _MethodPointer_ &method) { _method = method; return this; }

    virtual Pointer run(const Pointer &receiver, const MessagePointer &message) {
        return ((*const_cast<Node *>(receiver.data())).*method())(message);
    }
private:
    _MethodPointer_ _method;
};

CHILD_POINTER_DEFINE(NativeMethod,);

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
