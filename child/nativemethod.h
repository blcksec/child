#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(NativeMethod,);

#define CHILD_NATIVE_METHOD(ARGS...) NativeMethodPointer(new NativeMethod(Node::context()->child("NativeMethod"), ##ARGS))

#define CHILD_ADD_NATIVE_METHOD(NAME, METHOD) \
NAME::root()->addChild(#METHOD, new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&NAME::_##METHOD##_)))

class NativeMethod : public Node {
    CHILD_DECLARATION(NativeMethod, Node);
public:
    NativeMethod(const Pointer &origin, const _MethodPointer_ &method = NULL) : Node(origin), _method(method) {}
    static void initRoot() { Node::root()->addChild("NativeMethod", root()); }
    virtual Pointer fork() const { return new NativeMethod(this, _method); }

    _MethodPointer_ method() const { return _method; }
    NativeMethodPointer setMethod(const _MethodPointer_ &method) { _method = method; return this; }
private:
    _MethodPointer_ _method;
};

CHILD_POINTER_DEFINITION(NativeMethod,);

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
