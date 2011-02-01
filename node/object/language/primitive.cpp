#include "node/object/language/primitive.h"
#include "node/object/message.h"
#include "node/object/language/block.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Primitive, Element, Language);

    void Primitive::initRoot() {
        CHILD_ADD_NATIVE_METHOD(Primitive, run_before);
        CHILD_ADD_NATIVE_METHOD(Primitive, run_body);
        CHILD_ADD_NATIVE_METHOD(Primitive, run_after);
        CHILD_ADD_NATIVE_METHOD(Primitive, run_between);
    }

    Primitive *Primitive::setNext(Primitive *next) {
        if(next != _next) {
            if(_next) {
                _next->_previous = NULL;
                removeAnonymousChild(_next);
            }
            _next = next;
            if(next) {
                next->_previous = this;
                addAnonymousChild(next);
            }
        }
        return next;
    }

    Primitive *Primitive::last() {
        Primitive *primitive = this;
        while(primitive->hasNext()) primitive = primitive->next();
        return primitive;
    }

    int Primitive::size() const {
        int result = 1;
        const Primitive *primitive = this;
        while(primitive->hasNext()) {
            primitive = primitive->next();
            result++;
        }
        return result;
    }

    Node *Primitive::run(Node *receiver) {
        Node *result;
        try {
            CHILD_PUSH_RUN(this);
            result = value()->run(receiver);
        } catch(const Primitive::Skip &skip) {
            return skip.result;
        }
        return hasNext() ? result->receive(next()) : result;
    }

    CHILD_DEFINE_NATIVE_METHOD(Primitive, run_before) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        Block *block = Block::dynamicCast(value());
        Section *section = block ? block->beforeSection() : NULL;
        return section ? section->run() : CHILD_NODE();
    }

    CHILD_DEFINE_NATIVE_METHOD(Primitive, run_body) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        return run();
    }

    CHILD_DEFINE_NATIVE_METHOD(Primitive, run_after) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        Block *block = Block::dynamicCast(value());
        Section *section = block ? block->afterSection() : NULL;
        return section ? section->run() : CHILD_NODE();
    }

    CHILD_DEFINE_NATIVE_METHOD(Primitive, run_between) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        Block *block = Block::dynamicCast(value());
        Section *section = block ? block->betweenSection() : NULL;
        return section ? section->run() : CHILD_NODE();
    }

    QString Primitive::toString(bool debug, short level) const {
        QString str;
        const Primitive *primitive = this;
        bool first = true;
        do {
            if(!first) str += " "; else first = false;
            str += primitive->Element::toString(debug, level);
            primitive = primitive->next();
        } while(primitive);
        return str;
    }
}

CHILD_END
