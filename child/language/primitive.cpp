#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Primitive, Element);

    void Primitive::initRoot() {
        Language::root()->addChild("Primitive", root());
    }

    Primitive *Primitive::setNext(Primitive *next) {
        if(next != _next) {
            if(_next) _next->_previous = NULL;
            _next = next;
            if(next) next->_previous = this;
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
