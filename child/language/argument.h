#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "child/pair.h"
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

        CHILD_FORK_METHOD(Argument, CHILD_FORK_IF_NOT_NULL(label()), CHILD_FORK_IF_NOT_NULL(value()));

        // aliases...
        Primitive *label() const { return key(); }
        void setLabel(Primitive *label) { setKey(label); }

        QString labelName() const;

        virtual Node *run(Node *receiver = context()) {
            return value()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
