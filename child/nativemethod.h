#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(NativeMethod,);

#define CHILD_NATIVE_METHOD(ARGS...) NativeMethodPointer(new NativeMethod(Node::context()->child("NativeMethod"), ##ARGS))

class MessagePointer;
typedef Pointer (Node::*MethodPtr)(const MessagePointer &);

class NativeMethod : public Node {
    CHILD_DECLARATION(NativeMethod, Node);
public:
    NativeMethod(const Pointer &origin, const MethodPtr &method = NULL) : Node(origin), _method(method) {}
    static void initRoot() { Node::root()->addChild("NativeMethod", root()); }
    virtual Pointer fork() const { return new NativeMethod(this, _method); }

    MethodPtr method() const { return _method; }
    NativeMethodPointer setMethod(const MethodPtr &method) { _method = method; return this; }
private:
    MethodPtr _method;
};

CHILD_POINTER_DEFINITION(NativeMethod,);

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
