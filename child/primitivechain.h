#ifndef CHILD_PRIMITIVECHAIN_H
#define CHILD_PRIMITIVECHAIN_H

#include "child/object.h"

#define CHILD_PRIMITIVECHAIN(EXPRESSION) static_cast<PrimitiveChain *>(EXPRESSION)

namespace Child {
    class PrimitiveChain : public Object {
    public:
        static PrimitiveChain *root();
        static PrimitiveChain *fork(Node *world) { return(CHILD_PRIMITIVECHAIN(world->child("PrimitiveChain"))->fork()); }

        virtual PrimitiveChain *fork() { notYetImplemented(); return(NULL); }
    private:
        static PrimitiveChain *_root;
    };
}

#endif // CHILD_PRIMITIVECHAIN_H
