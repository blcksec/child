#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/object.h"

#define CHILD_BLOCK(EXPRESSION) static_cast<Block *>(EXPRESSION)

namespace Child {
    class Block : public Object {
    public:
        static Block *root();
        static Block *fork(Node *world) { return(CHILD_BLOCK(world->child("Block"))->fork()); }

        virtual Block *fork() { notYetImplemented(); return(NULL); }
    private:
        static Block *_root;
    };
}

#endif // CHILD_BLOCK_H
