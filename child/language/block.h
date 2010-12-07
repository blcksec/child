#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/language/section.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Block, List);

    #define CHILD_BLOCK(ARGS...) \
    BlockPtr(new Block(Node::findInContext("Object")->child("Language")->child("Block"), ##ARGS))

    class Block : public GenericList<BlockPtr, SectionPtr> {
        CHILD_DECLARATION(Block, List);
    public:
        Block(const NodePtr &origin) : GenericList<BlockPtr, SectionPtr>(origin) {}

        static void initRoot() { Language::root()->addChild("Block", root()); }
        virtual NodePtr fork() const { CHILD_TODO; return new Block(this); }

        virtual const QString toString(bool debug = false) const { return join("\n", "", "", debug); }
    };

    CHILD_PTR_DEFINITION(Block, List);
}

CHILD_END

#endif // CHILD_BLOCK_H
