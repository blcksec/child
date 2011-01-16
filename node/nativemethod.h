#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "node.h"
#include "node/exception.h"

CHILD_BEGIN

#define CHILD_NATIVE_METHOD(ARGS...) new NativeMethod(context()->child("NativeMethod"), ##ARGS)

#define CHILD_ADD_NATIVE_METHOD(CLASS, METHOD, NAME...) \
addOrSetChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_)))

typedef Node *(Node::*_MethodPointer_)();

class NativeMethod : public Node {
    CHILD_DECLARE(NativeMethod, Node, Node);
public:
    explicit NativeMethod(Node *origin, const _MethodPointer_ &method = NULL) :
        Node(origin) { setMethod(method); }

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(NativeMethod, method());

    _MethodPointer_ method() const { return _method; }

    void setMethod(const _MethodPointer_ &method) {
        _method = method;
        setIsRunnable(method);
    }

    virtual Node *run(Node *receiver) {
        CHILD_PUSH_RUN(this);
        return (receiver->*method())();
    }
private:
    _MethodPointer_ _method;
};

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
