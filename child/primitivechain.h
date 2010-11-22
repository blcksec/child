#ifndef CHILD_PRIMITIVECHAIN_H
#define CHILD_PRIMITIVECHAIN_H

#include "child/object.h"
#include "child/list.h"
#include "child/primitive.h"

namespace Child {
    class PrimitiveChain : public Object {
        CHILD_DECLARATION(PrimitiveChain);
    public:
        PrimitiveChain() : _primitives(NULL) {}

        virtual ~PrimitiveChain() {
            delete _primitives;
        }

        List *primitives(bool createIfNull = false) const {
            if(_primitives) return(_primitives);
            if(createIfNull)
                return(const_cast<PrimitiveChain *>(this)->_primitives =
                       List::fork(const_cast<PrimitiveChain *>(this)));
            else
                return(List::root());
        }

        void append(Primitive *primitive) {
            if(!primitive) throw(NullPointerException("Primitive pointer is NULL"));
            primitives(true)->append(primitive);
        }

        Node *get(int i) const { return(Primitive::as(primitives()->get(i))->value()); }
        Node *first() const { return(get(0)); }
        Node *second() const { return(get(1)); }
        Node *last() const { return(get(size()-1)); }
        const bool isEmpty() const { return(primitives()->isEmpty()); }
        const bool isNotEmpty() const { return(primitives()->isNotEmpty()); }
        int size() const { return(primitives()->size()); }

        virtual const QString inspect() const {
            QString str;
            str.append("(");
            for(int i = 0; i < primitives()->size(); i++) {
                if(i > 0) str.append(" ");
                str.append(Primitive::as(primitives()->get(i))->inspect());
            }
            str.append(")");
            return(str);
        }
    private:
        List *_primitives;
    };
}

#endif // CHILD_PRIMITIVECHAIN_H
