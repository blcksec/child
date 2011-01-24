#ifndef CHILD_LANGUAGE_ARGUMENT_H
#define CHILD_LANGUAGE_ARGUMENT_H

#include "node/object/pair.h"
#include "node/object/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_ARGUMENT(ARGS...) \
    new Language::Argument(context()->child("Object", "Language", "Argument"), ##ARGS)

    class Argument : public GenericPair<Primitive *, Primitive *> {
        CHILD_DECLARE(Argument, Pair, Language);
    public:
        explicit Argument(Node *origin) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, NULL) {}

        Argument(Node *origin, Primitive *value) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, NULL) { setValue(value); }

        Argument(Node *origin, Primitive *label, Primitive *value) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, NULL) { setLabel(label); setValue(value); }

        Argument(Node *origin, Node *node) :
            GenericPair<Primitive *, Primitive *>(origin, NULL, NULL) { setValue(CHILD_PRIMITIVE(node)); }

        virtual ~Argument() {
            setLabel(NULL);
            setValue(NULL);
        }

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Argument);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Argument, CHILD_FORK_IF_NOT_NULL(label()), CHILD_FORK_IF_NOT_NULL(value()));

        virtual void firstValueWillChange() { if(first()) removeAnonymousChild(first()); }
        virtual void firstValueHasChanged() { if(first()) addAnonymousChild(first()); }
        virtual void secondValueWillChange() { if(second()) removeAnonymousChild(second()); }
        virtual void secondValueHasChanged() { if(second()) addAnonymousChild(second()); }

        // aliases...
        Primitive *label() const { return key(); }
        void setLabel(Primitive *label) { setKey(label); }

        QString labelName() const;

        virtual Node *run(Node *receiver = context()) {
            // CHILD_PUSH_RUN(this); // is it really necessary?
            return value()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_H
