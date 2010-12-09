#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/language/section.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Block, List);

#define CHILD_BLOCK(ARGS...) \
BlockPtr(new Block(Node::context()->child("Object", "Block"), ##ARGS))

class Block : public GenericList<BlockPtr, Language::SectionPtr> {
    CHILD_DECLARATION(Block, List);
public:
    Block(const NodePtr &origin) : GenericList<BlockPtr, Language::SectionPtr>(origin) {}

    static void initRoot() { Object::root()->addChild("Block", root()); }
    virtual NodePtr fork() const { return BlockPtr(new Block(this))->initFork(); }

    virtual const QString toString(bool debug = false) const { return join("\n", "", "", debug); }
};

CHILD_PTR_DEFINITION(Block, List);

CHILD_END

#endif // CHILD_BLOCK_H
