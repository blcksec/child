#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "child/pair.h"
#include "child/bunch.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Argument, Pair);

    #define CHILD_ARGUMENT(ARGS...) \
    Language::ArgumentPtr(new Language::Argument(Node::context()->child("Object", "Language", "Argument"), ##ARGS))

    class Argument : public GenericPair<ArgumentPtr, PrimitiveChainPtr, PrimitiveChainPtr> {
        CHILD_DECLARATION(Argument, Pair);
    public:
        Argument(const NodePtr &origin, const PrimitiveChainPtr &label = NULL, const PrimitiveChainPtr &value = NULL) :
            GenericPair<ArgumentPtr, PrimitiveChainPtr, PrimitiveChainPtr>(origin, label, value) {}

        static void initRoot() { Language::root()->addChild("Argument", root()); }

        virtual NodePtr fork() const {
            return new Argument(this,
                                label() ? label()->fork() : NodePtr::null(),
                                value() ? value()->fork() : NodePtr::null());
        }

        // aliases...
        PrimitiveChainPtr label() const { return key(); }
        void setLabel(const PrimitiveChainPtr &label) { setKey(label); }

        virtual const QString toString(bool debug = false) const {
            return concatenateStrings(label() ? label()->toString(debug) + ":" : "",
                                      " ",
                                      value() ? value()->toString(debug) : "");
        }
    };

    CHILD_PTR_DEFINITION(Argument, Pair);

    // === ArgumentBunch ===

    CHILD_PTR_DECLARATION(ArgumentBunch, Bunch);

    #define CHILD_ARGUMENT_BUNCH(ARGS...) \
    Language::ArgumentBunchPtr(new Language::ArgumentBunch( \
        Node::context()->child("Object", "Language", "ArgumentBunch"), ##ARGS))

    class ArgumentBunch : public GenericList<ArgumentBunchPtr, ArgumentPtr> {
        CHILD_DECLARATION(ArgumentBunch, Bunch);
    public:
        ArgumentBunch(const NodePtr &origin, const QList<ArgumentPtr> &other = QList<ArgumentPtr>()) :
            GenericList<ArgumentBunchPtr, ArgumentPtr>(origin, other, true) {}

        static void initRoot() { Language::root()->addChild("ArgumentBunch", root()); }
        virtual NodePtr fork() const { return ArgumentBunchPtr(new ArgumentBunch(this))->initFork(); }

        ArgumentPtr append(const ArgumentPtr &argument) { // Strange... Shoud be inherited from GenericList?
            return GenericList<ArgumentBunchPtr, ArgumentPtr>::append(argument);
        }

        ArgumentPtr append(const PrimitiveChainPtr &label, const PrimitiveChainPtr &value) {
            return GenericList<ArgumentBunchPtr, ArgumentPtr>::append(CHILD_ARGUMENT(label, value));
        }

        virtual const QString toString(bool debug = false) const { return join(", ", "", "", debug); }
    };

    CHILD_PTR_DEFINITION(ArgumentBunch, Bunch);
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
