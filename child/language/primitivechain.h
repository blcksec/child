#ifndef CHILD_PRIMITIVECHAIN_H
#define CHILD_PRIMITIVECHAIN_H

#include "child/list.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(PrimitiveChain, List);

    #define CHILD_PRIMITIVE_CHAIN(ARGS...) \
    PrimitiveChainPtr(new PrimitiveChain(Node::findInContext("Object")->child("Language")->child("PrimitiveChain"), ##ARGS))

    class PrimitiveChain : public GenericList<PrimitiveChainPtr, PrimitivePtr> {
        CHILD_DECLARATION(PrimitiveChain, List);
    public:
        PrimitiveChain(const NodePtr &origin) : GenericList<PrimitiveChainPtr, PrimitivePtr>(origin) {}

        static void initRoot() { Language::root()->addChild("PrimitiveChain", root()); }
        virtual NodePtr fork() const { CHILD_TODO; return new PrimitiveChain(this); }

        virtual const QString inspect() const {
            QString str = "(";
            bool first = true;
            Iterator i(this);
            while(PrimitivePtr prim = i.next()) {
                if(!first) str += " "; else first = false;
                str += prim->inspect();
            }
            str += ")";
            return str;
        }
    };

    CHILD_PTR_DEFINITION(PrimitiveChain, List);
}

CHILD_END

#endif // CHILD_PRIMITIVECHAIN_H
