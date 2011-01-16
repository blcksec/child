#include "node/object/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Primitive, Element, Language);

    void Primitive::initRoot() {
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
