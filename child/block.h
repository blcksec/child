#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/language/section.h"

CHILD_BEGIN

#define CHILD_BLOCK(ARGS...) \
new Block(Node::context()->child("Object", "Block"), ##ARGS)

class Block : public GenericList<Section *> {
    CHILD_DECLARE(Block, List);
public:
    explicit Block(Node *origin) : GenericList<Section *>(origin),
        _doc(NULL), _docIsCached(false), _body(NULL), _bodyIsCached(false), _else(NULL), _elseIsCached(false) {}

    CHILD_FORK_METHOD(Block);

    virtual Node *run(Node *receiver = context()) {
        return bodySection() ? bodySection()->run(receiver) : NULL;
    }

    Section *section(const QString &label) {
        if(label.isEmpty()) return hasUnlabeledSection();
        else if(label == "doc") return docSection();
        else if(label == "body") return bodySection();
        else if(label == "else") return elseSection();
        else return findSection(label);
    }

    Section *docSection() {
        if(!_docIsCached) {
            _doc = findSection("doc");
            if(!_doc && findSection("body")) _doc = hasUnlabeledSection();
            _docIsCached = true;
        }
        return _doc;
    }

    Section *bodySection() {
        if(!_bodyIsCached) {
            _body = findSection("body");
            if(!_body) _body = hasUnlabeledSection();
            _bodyIsCached = true;
        }
        return _body;
    }

    Section *elseSection() {
        if(!_elseIsCached) {
            _else = findSection("else");
            _elseIsCached = true;
        }
        return _else;
    }
private:
    Section *findSection(const QString &label);
public:
    Section *hasUnlabeledSection() {
        if(isNotEmpty() && !first()->label())
            return first();
        else
            return NULL;
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str;
        if(level > 0) str += "{\n";
        str += join("\n", "", "", debug, level + 1);
        if(level > 0) str += "\n" + QString("    ").repeated(level) + "}";
        return str;
    }
private:
    Section *_doc;
    bool _docIsCached;
    Section *_body;
    bool _bodyIsCached;
    Section *_else;
    bool _elseIsCached;
};

CHILD_END

#endif // CHILD_BLOCK_H
