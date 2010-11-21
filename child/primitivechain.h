#ifndef CHILD_PRIMITIVECHAIN_H
#define CHILD_PRIMITIVECHAIN_H

#include "child/object.h"
#include "child/list.h"
#include "child/primitive.h"

#define CHILD_PRIMITIVECHAIN(EXPRESSION) static_cast<PrimitiveChain *>(EXPRESSION)

namespace Child {
    class PrimitiveChain : public Object {
    public:
        static PrimitiveChain *root();
        static PrimitiveChain *fork(Node *world) { return(CHILD_PRIMITIVECHAIN(world->child("PrimitiveChain"))->fork()); }

        PrimitiveChain() : _primitives(NULL) {}

        virtual ~PrimitiveChain() {
            delete _primitives;
        }

        virtual PrimitiveChain *fork() { return(_fork(this)); }

        List *primitives() const { return(_primitives); }

        void append(Primitive *primitive) {
            if(!primitive) throw(NullPointerException("Primitive pointer is NULL"));
            if(!_primitives) _primitives = List::fork(this);
            _primitives->append(primitive);
        }

        virtual const QString inspect() const {
            QString str;
            str.append("(");
            if(primitives()) {
                for(int i = 0; i < primitives()->size(); i++) {
                    if(i > 0) str.append(" ");
                    str.append(CHILD_PRIMITIVE(primitives()->get(i))->inspect());
                }
            }
            str.append(")");
            return(str);
        }
    private:
        static PrimitiveChain *_root;
        List *_primitives;
    };
}

#endif // CHILD_PRIMITIVECHAIN_H
