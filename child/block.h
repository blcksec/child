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
    explicit Block(const Pointer &origin) : GenericList<BlockPointer, SectionPointer>(origin),
        _docIsCached(false), _bodyIsCached(false), _elseIsCached(false) {}

    static void initRoot() { Object::root()->addChild("Block", root()); }
    virtual Pointer fork() const { return BlockPointer(new Block(this))->initFork(); }

    virtual Pointer run(const Pointer &receiver = context()) {
        return bodySection() ? bodySection()->run(receiver) : Pointer::null();
    }

    SectionPointer section(const QString &label) {
        if(label.isEmpty()) return hasUnlabeledSection();
        else if(label == "doc") return docSection();
        else if(label == "body") return bodySection();
        else if(label == "else") return elseSection();
        else return findSection(label);
    }

    SectionPointer docSection() {
        if(!_docIsCached) {
            _doc = findSection("doc");
            if(!_doc && findSection("body")) _doc = hasUnlabeledSection();
            _docIsCached = true;
        }
        return _doc;
    }

    SectionPointer bodySection() {
        if(!_bodyIsCached) {
            _body = findSection("body");
            if(!_body) _body = hasUnlabeledSection();
            _bodyIsCached = true;
        }
        return _body;
    }

    SectionPointer elseSection() {
        if(!_elseIsCached) {
            _else = findSection("else");
            _elseIsCached = true;
        }
        return _else;
    }
private:
    SectionPointer findSection(const QString &label);
public:
    SectionPointer hasUnlabeledSection() {
        if(isNotEmpty() && (!first()->label() || first()->label()->isEmpty()))
            return first();
        else
            return SectionPointer::null();
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str;
        if(level > 0) str += "{\n";
        str += join("\n", "", "", debug, level + 1);
        if(level > 0) str += "\n" + QString("    ").repeated(level) + "}";
        return str;
    }
private:
    SectionPointer _doc;
    bool _docIsCached;
    SectionPointer _body;
    bool _bodyIsCached;
    SectionPointer _else;
    bool _elseIsCached;
};

CHILD_POINTER_DEFINE(Block, List);

CHILD_END

#endif // CHILD_BLOCK_H
