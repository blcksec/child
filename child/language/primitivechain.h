#ifndef CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
#define CHILD_LANGUAGE_PRIMITIVE_CHAIN_H

#include "child/list.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(PrimitiveChain, List);

    #define CHILD_PRIMITIVE_CHAIN(ARGS...) \
    Language::PrimitiveChainPointer(new Language::PrimitiveChain( \
        Node::context()->child("Object", "Language", "PrimitiveChain"), ##ARGS))

    class PrimitiveChain : public GenericList<PrimitiveChainPointer, PrimitivePointer> {
        CHILD_DECLARE(PrimitiveChain, List);
    public:
        PrimitiveChain(const Pointer &origin) :
            GenericList<PrimitiveChainPointer, PrimitivePointer>(origin) {}

        PrimitiveChain(const Pointer &origin, const PrimitivePointer &primitive) :
            GenericList<PrimitiveChainPointer, PrimitivePointer>(origin, primitive) {}

        static void initRoot() { Language::root()->addChild("PrimitiveChain", root()); }
        virtual Pointer fork() const { return PrimitiveChainPointer(new PrimitiveChain(this))->initFork(); }

        virtual Pointer run(const Pointer &receiver = context()) {
            Pointer result;
            Pointer currentReceiver = receiver;
            Iterator i(this);
            while(PrimitivePointer primitive = i.next()) {
                result = primitive->run(currentReceiver);
                currentReceiver = result;
            }
            return result;
        }

        virtual const QString toString(bool debug = false, short level = 0) const { return join(" ", "", "", debug, level); }
    };

    CHILD_POINTER_DEFINE(PrimitiveChain, List);
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
