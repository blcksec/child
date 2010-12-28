#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "child/pair.h"
#include "child/bunch.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_ARGUMENT(ARGS...) \
    new Language::Argument(Node::context()->child("Object", "Language", "Argument"), ##ARGS)

    class Argument : public GenericPair<PrimitiveChain *, PrimitiveChain *> {
        CHILD_DECLARE(Argument, Pair);
    public:
        explicit Argument(const Node *origin) :
            GenericPair<PrimitiveChain *, PrimitiveChain *>(origin) {}

        Argument(const Node *origin, const PrimitiveChain *value) :
            GenericPair<PrimitiveChain *, PrimitiveChain *>(origin, NULL, value) {}

        Argument(const Node *origin, const PrimitiveChain *label, const PrimitiveChain *value) :
            GenericPair<PrimitiveChain *, PrimitiveChain *>(origin, label, value) {}

        Argument(const Node *origin, const Node *node) :
            GenericPair<PrimitiveChain *, PrimitiveChain *>(origin, NULL, CHILD_PRIMITIVE_CHAIN(node)) {}

        static void initRoot() { Language::root()->addChild("Argument", root()); }

        virtual Node *fork() const {
            return new Argument(this, forkIfNotNull(label()), forkIfNotNull(value()));
        }

        // aliases...
        PrimitiveChain *label() const { return key(); }
        void setLabel(const PrimitiveChain *label) { setKey(label); }

        QString labelName() const;

        virtual Node *run(const Node *receiver = context()) {
            return value()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return concatenateStrings(label() ? label()->toString(debug, level) + ":" : "",
                                      " ",
                                      value() ? value()->toString(debug, level) : "");
        }
    };

    // === ArgumentBunch ===

    #define CHILD_ARGUMENT_BUNCH(ARGS...) \
    new Language::ArgumentBunch(Node::context()->child("Object", "Language", "ArgumentBunch"), ##ARGS)

    #define CHILD_CHECK_INPUT_SIZE(ARGS...) \
    ArgumentBunch::checkSpecifiedSize(message->inputs(false) ? message->inputs()->size() : 0, ##ARGS)

    class ArgumentBunch : public GenericList<Argument *> {
        CHILD_DECLARE(ArgumentBunch, Bunch);
    public:
        explicit ArgumentBunch(const Node *origin) : GenericList<Argument *>(origin, true) {}

        ArgumentBunch(const Node *origin, const Argument *argument) :
            GenericList<Argument *>(origin, argument, true) {}

        ArgumentBunch(const Node *origin, const Argument *argument1, const Argument *argument2) :
            GenericList<Argument *>(origin, argument1, argument2, true) {}

        ArgumentBunch(const Node *origin, const Node *argument) :
            GenericList<Argument *>(origin, CHILD_ARGUMENT(argument), true) {}

        ArgumentBunch(const Node *origin, const Node *argument1, const Node *argument2) :
            GenericList<Argument *>(
                origin, CHILD_ARGUMENT(argument1), CHILD_ARGUMENT(argument2), true) {}

        static void initRoot() { Language::root()->addChild("ArgumentBunch", root()); }
        virtual Node *fork() const { return (new ArgumentBunch(this))->initFork(); }

        using GenericList<Argument *>::append;

        void append(const PrimitiveChain *value) {
            if(Pair *pair = Pair::dynamicCast(value->first()->value()))
                append(pair->first(), pair->second());
            else if(Bunch *bunch = Bunch::dynamicCast(value->first()->value()))
                append(bunch);
            else
                append(CHILD_ARGUMENT(NULL, value));
        }

        Argument *append(const PrimitiveChain *label, const PrimitiveChain *value) {
            return append(CHILD_ARGUMENT(label, value));
        }

        void append(const Bunch *bunch) {
            Bunch::Iterator i(bunch);
            while(Node *value = i.next()) append(PrimitiveChain::cast(value));
        }

        void checkSize(short min, short max = -1) { checkSpecifiedSize(size(), min, max); }
        static void checkSpecifiedSize(short size, short min, short max = -1);

        virtual QString toString(bool debug = false, short level = 0) const { return join(", ", "", "", debug, level); }
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
