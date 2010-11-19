#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/object.h"

#define CHILD_BLOCK(EXPRESSION) static_cast<Block *>(EXPRESSION)

namespace Child {
    class Block : public Object {
    public:
        static Block *root();
    private:
        static Block *_root;
    };
}

#endif // CHILD_BLOCK_H
