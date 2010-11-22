#ifndef CHILD_NATIVEMETHOD_H
#define CHILD_NATIVEMETHOD_H

#include "child/node.h"

#define CHILD_METHODPTR(METH) static_cast<NodeMethodPtr>(&METH)

namespace Child {
    class NativeMethod : public Node {
        CHILD_DECLARATION(NativeMethod);
    public:
        static NativeMethod *fork(Node *world, NodeMethodPtr method) {
            return(NativeMethod::fork(world)->setMethod(method));
        }

        NativeMethod() : _method(NULL) {}

        virtual void initFork() {
            setMethod(NativeMethod::as(origin())->_method);
        }

        NodeMethodPtr method() const { return(_method); }
        NativeMethod *setMethod(NodeMethodPtr method) { _method = method; return(this); }
    private:
        NodeMethodPtr _method;
    };
}

#endif // CHILD_NATIVEMETHOD_H
