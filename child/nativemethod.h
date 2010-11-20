#ifndef CHILD_NATIVEMETHOD_H
#define CHILD_NATIVEMETHOD_H

#include "child/node.h"

#define CHILD_NATIVEMETHOD(EXPRESSION) static_cast<NativeMethod *>(EXPRESSION)
#define CHILD_METHODPTR(METH) static_cast<NodeMethodPtr>(&METH)

namespace Child {
    class NativeMethod : public Node {
    public:
        static NativeMethod *root();
        static NativeMethod *fork(Node *world) { return(CHILD_NATIVEMETHOD(world->child("NativeMethod"))->fork()); }
        static NativeMethod *fork(Node *world, NodeMethodPtr method) { return(CHILD_NATIVEMETHOD(world->child("NativeMethod"))->fork(method)); }

        NativeMethod() : _method(NULL) {}

        virtual NativeMethod *fork() { return(_fork(this)->setMethod(_method)); }
        virtual NativeMethod *fork(NodeMethodPtr method) { return(_fork(this)->setMethod(method)); }

        NodeMethodPtr method() const { return(_method); }
        NativeMethod *setMethod(NodeMethodPtr method) { _method = method; return(this); }
    private:
        static NativeMethod *_root;
        NodeMethodPtr _method;
    };
}

#endif // CHILD_NATIVEMETHOD_H
