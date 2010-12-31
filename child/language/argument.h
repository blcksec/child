#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "child/pair.h"
#include "child/bunch.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_ARGUMENT(ARGS...) \
    new Language::Argument(Node::context()->child("Object", "Language", "Argument"), ##ARGS)

    class Argument : public GenericPair<Primitive *, Primitive *> {
        CHILD_DECLARE(Argument, Pair);
    public:
        explicit Argument(Node *origin) :
            GenericPair<Primitive *, Primitive *>(origin) {}

        Argument(Node *origin, Primitive *value) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, value) {}

        Argument(Node *origin, Primitive *label, Primitive *value) :
            GenericPair<Primitive *, Primitive *>(origin, label, value) {}

        Argument(Node *origin, Node *node) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, CHILD_PRIMITIVE(node)) {}

        static void initRoot() { Language::root()->addChild("Argument", root()); }

        CHILD_FORK_METHOD(Argument, CHILD_FORK_IF_NOT_NULL(label()), CHILD_FORK_IF_NOT_NULL(value()));

        // aliases...
        Primitive *label() const { return key(); }
        void setLabel(Primitive *label) { setKey(label); }

        QString labelName() const;

        virtual Node *run(Node *receiver = context()) {
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
        explicit ArgumentBunch(Node *origin) : GenericList<Argument *>(origin, true) {}

        ArgumentBunch(Node *origin, Argument *argument) :
            GenericList<Argument *>(origin, argument, true) {}

        ArgumentBunch(Node *origin, Argument *argument1, Argument *argument2) :
            GenericList<Argument *>(origin, argument1, argument2, true) {}

        ArgumentBunch(Node *origin, Node *argument) :
            GenericList<Argument *>(origin, CHILD_ARGUMENT(argument), true) {}

        ArgumentBunch(Node *origin, Node *argument1, Node *argument2) :
            GenericList<Argument *>(
                origin, CHILD_ARGUMENT(argument1), CHILD_ARGUMENT(argument2), true) {}

        static void initRoot() { Language::root()->addChild("ArgumentBunch", root()); }

        CHILD_FORK_METHOD(ArgumentBunch);

        using GenericList<Argument *>::append;

        void append(Primitive *value) {
            if(Pair *pair = Pair::dynamicCast(value->value()))
                append(Primitive::cast(pair->first()), Primitive::cast(pair->second()));
            else if(Bunch *bunch = Bunch::dynamicCast(value->value()))
                append(bunch);
            else
                append(CHILD_ARGUMENT(NULL, value));
        }

        Argument *append(Primitive *label, Primitive *value) {
            return append(CHILD_ARGUMENT(label, value));
        }

        void append(const Bunch *bunch) {
            Bunch::Iterator i(bunch);
            while(Node *value = i.next()) append(Primitive::cast(value));
        }

        void checkSize(short min, short max = -1) { checkSpecifiedSize(size(), min, max); }
        static void checkSpecifiedSize(short size, short min, short max = -1);

        virtual QString toString(bool debug = false, short level = 0) const { return join(", ", "", "", debug, level); }
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
