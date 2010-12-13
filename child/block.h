#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/language/section.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Block, List);

#define CHILD_BLOCK(ARGS...) \
BlockPointer(new Block(Node::context()->child("Object", "Block"), ##ARGS))

class Block : public GenericList<BlockPointer, Language::SectionPointer> {
    CHILD_DECLARATION(Block, List);
public:
    Block(const Pointer &origin) : GenericList<BlockPointer, Language::SectionPointer>(origin) {}

    static void initRoot() { Object::root()->addChild("Block", root()); }
    virtual Pointer fork() const { return BlockPointer(new Block(this))->initFork(); }

    virtual const QString toString(bool debug = false, short level = 0) const {
        QString str;
        if(level > 0) str += "{\n";
        str += join("\n", "", "", debug, level + 1);
        if(level > 0) str += "\n" + QString("    ").repeated(level) + "}";
        return str;
    }
};

CHILD_POINTER_DEFINITION(Block, List);

CHILD_END

#endif // CHILD_BLOCK_H
