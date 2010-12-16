#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/language/section.h"

CHILD_BEGIN

using namespace Language;

CHILD_POINTER_DECLARE(Block, List);

#define CHILD_BLOCK(ARGS...) \
BlockPointer(new Block(Node::context()->child("Object", "Block"), ##ARGS))

class Block : public GenericList<BlockPointer, SectionPointer> {
    CHILD_DECLARE(Block, List);
public:
    Block(const Pointer &origin) : GenericList<BlockPointer, SectionPointer>(origin) {}

    static void initRoot() { Object::root()->addChild("Block", root()); }
    virtual Pointer fork() const { return BlockPointer(new Block(this))->initFork(); }

    virtual Pointer run(const Pointer &receiver = context()) {
        return bodySection()->run(receiver);
    }

    SectionPointer docSection() {
        if(!_doc) {
            SectionPointer unlabeledSection = hasUnlabeledSection();
            if(SectionPointer doc = findSection("doc")) {
                if(unlabeledSection)
                    CHILD_THROW(DuplicateException, "a block cannot have both an unlabeled section and an explicit doc section"); // TODO: proper error report with resource filename and line number
                _doc = doc;
            } else if(size() > 1 && unlabeledSection)
                _doc = unlabeledSection;
            else
                _doc = Section::empty();
        }
        return _doc;
    }

    SectionPointer bodySection() {
        if(!_body) {
            if(size() == 1 && hasUnlabeledSection())
                _body = first();
            else {
                _body = findSection("body");
                if(!_body) _body = Section::empty();
            }
        }
        return _body;
    }

    SectionPointer elseSection() {
        if(!_else) {
            _else = findSection("else");
            if(!_else) _else = Section::empty();
        }
        return _else;
    }

    SectionPointer findSection(const QString &label);

    SectionPointer hasUnlabeledSection() {
        if(isNotEmpty() && (!first()->label() || first()->label()->isEmpty()))
            return first();
        else
            return SectionPointer::null();
    }

    virtual const QString toString(bool debug = false, short level = 0) const {
        QString str;
        if(level > 0) str += "{\n";
        str += join("\n", "", "", debug, level + 1);
        if(level > 0) str += "\n" + QString("    ").repeated(level) + "}";
        return str;
    }
private:
    SectionPointer _doc;
    SectionPointer _body;
    SectionPointer _else;
};

CHILD_POINTER_DEFINE(Block, List);

CHILD_END

#endif // CHILD_BLOCK_H
