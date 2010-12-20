#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "child/pair.h"
#include "child/bunch.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(Argument, Pair);

    #define CHILD_ARGUMENT(ARGS...) \
    Language::ArgumentPointer(new Language::Argument(Node::context()->child("Object", "Language", "Argument"), ##ARGS))

    class Argument : public GenericPair<ArgumentPointer, PrimitiveChainPointer, PrimitiveChainPointer> {
        CHILD_DECLARE(Argument, Pair);
    public:
        Argument(const Pointer &origin, const PrimitiveChainPointer &label = NULL, const PrimitiveChainPointer &value = NULL) :
            GenericPair<ArgumentPointer, PrimitiveChainPointer, PrimitiveChainPointer>(origin, label, value) {}

        static void initRoot() { Language::root()->addChild("Argument", root()); }

        virtual Pointer fork() const {
            return new Argument(this, forkIfNotNull(label()), forkIfNotNull(value()));
        }

        // aliases...
        PrimitiveChainPointer label() const { return key(); }
        void setLabel(const PrimitiveChainPointer &label) { setKey(label); }

        virtual Pointer run(const Pointer &receiver = context()) {
            return value()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return concatenateStrings(label() ? label()->toString(debug, level) + ":" : "",
                                      " ",
                                      value() ? value()->toString(debug, level) : "");
        }
    };

    CHILD_POINTER_DEFINE(Argument, Pair);

    // === ArgumentBunch ===

    CHILD_POINTER_DECLARE(ArgumentBunch, Bunch);

    #define CHILD_ARGUMENT_BUNCH(ARGS...) \
    Language::ArgumentBunchPointer(new Language::ArgumentBunch( \
        Node::context()->child("Object", "Language", "ArgumentBunch"), ##ARGS))

    #define CHILD_CHECK_INPUT_SIZE(ARGS...) \
    ArgumentBunch::checkSpecifiedSize(message->inputs(false) ? message->inputs()->size() : 0, ##ARGS)

    class ArgumentBunch : public GenericList<ArgumentBunchPointer, ArgumentPointer> {
        CHILD_DECLARE(ArgumentBunch, Bunch);
    public:
        ArgumentBunch(const Pointer &origin) : GenericList<ArgumentBunchPointer, ArgumentPointer>(origin, true) {}

        static void initRoot() { Language::root()->addChild("ArgumentBunch", root()); }
        virtual Pointer fork() const { return ArgumentBunchPointer(new ArgumentBunch(this))->initFork(); }

        using GenericList<ArgumentBunchPointer, ArgumentPointer>::append;

        void append(const PrimitiveChainPointer &value) {
            if(PairPointer pair = PairPointer(value->first()->value(), true))
                append(pair->first(), pair->second());
            else if(BunchPointer bunch = BunchPointer(value->first()->value(), true))
                append(bunch);
            else
                append(CHILD_ARGUMENT(NULL, value));
        }

        ArgumentPointer append(const PrimitiveChainPointer &label, const PrimitiveChainPointer &value) {
            return append(CHILD_ARGUMENT(label, value));
        }

        void append(const BunchPointer &bunch) {
            Bunch::Iterator i(bunch);
            while(Pointer value = i.next()) append(PrimitiveChainPointer(value));
        }

        void checkSize(short min, short max = -1) { checkSpecifiedSize(size(), min, max); }
        static void checkSpecifiedSize(short size, short min, short max = -1);

        virtual QString toString(bool debug = false, short level = 0) const { return join(", ", "", "", debug, level); }
    };

    CHILD_POINTER_DEFINE(ArgumentBunch, Bunch);
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
