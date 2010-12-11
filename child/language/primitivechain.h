#ifndef CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
#define CHILD_LANGUAGE_PRIMITIVE_CHAIN_H

#include "child/list.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(PrimitiveChain, List);

    #define CHILD_PRIMITIVE_CHAIN(ARGS...) \
    Language::PrimitiveChainPtr(new Language::PrimitiveChain( \
        Node::context()->child("Object", "Language", "PrimitiveChain"), ##ARGS))

    class PrimitiveChain : public GenericList<PrimitiveChainPtr, PrimitivePtr> {
        CHILD_DECLARATION(PrimitiveChain, List);
    public:
        PrimitiveChain(const NodePtr &origin) :
            GenericList<PrimitiveChainPtr, PrimitivePtr>(origin) {}

        PrimitiveChain(const NodePtr &origin, const PrimitivePtr &primitive) :
            GenericList<PrimitiveChainPtr, PrimitivePtr>(origin, primitive) {}

        static void initRoot() { Language::root()->addChild("PrimitiveChain", root()); }
        virtual NodePtr fork() const { return PrimitiveChainPtr(new PrimitiveChain(this))->initFork(); }

        virtual const QString toString(bool debug = false) const { return join(" ", "", "", debug); }
    };

    CHILD_PTR_DEFINITION(PrimitiveChain, List);
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
