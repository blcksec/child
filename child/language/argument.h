#ifndef CHILD_ARGUMENT_H
#define CHILD_ARGUMENT_H

#include "child/pair.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Argument, Pair);

    #define CHILD_ARGUMENT(ARGS...) \
    Language::ArgumentPtr(new Language::Argument(Node::findInContext("Object")->child("Language")->child("Argument"), ##ARGS))

    class Argument : public GenericPair<ArgumentPtr, PrimitiveChainPtr, PrimitiveChainPtr> {
        CHILD_DECLARATION(Argument, Pair);
    public:
        Argument(const NodePtr &origin, PrimitiveChainPtr label = NULL, PrimitiveChainPtr value = NULL) :
            GenericPair<ArgumentPtr, PrimitiveChainPtr, PrimitiveChainPtr>(origin, label, value) {}

        static void initRoot() { Language::root()->addChild("Argument", root()); }
        virtual NodePtr fork() const { return new Argument(this, label()->fork(), value()->fork()); }

        // aliases...
        PrimitiveChainPtr label() const { return key(); }
        void setLabel(const PrimitiveChainPtr &label) { setKey(label); }
    };

    CHILD_PTR_DEFINITION(Argument, Pair);
}

CHILD_END

#endif // CHILD_ARGUMENT_H
