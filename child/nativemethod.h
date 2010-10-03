#ifndef NATIVEMETHOD_H
#define NATIVEMETHOD_H

#include "child/node.h"

#define CHILD_METHODPTR(METH) static_cast<NodeMethodPtr>(&METH)

namespace Child {
    class NativeMethod : public Node {
    public:
        NativeMethod() : _method(NULL) {}

        static NativeMethod *root();

        virtual NativeMethod *fork() { return(_fork(this)->setMethod(_method)); }
        virtual NativeMethod *fork(NodeMethodPtr method) { return(_fork(this)->setMethod(method)); }

        NodeMethodPtr method() const { return(_method); }
        NativeMethod *setMethod(NodeMethodPtr method) { _method = method; return(this); }
    private:
        static NativeMethod *_root;
        NodeMethodPtr _method;
    };
}

#endif // NATIVEMETHOD_H
