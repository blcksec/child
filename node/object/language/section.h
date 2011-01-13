#ifndef CHILD_LANGUAGE_SECTION_H
#define CHILD_LANGUAGE_SECTION_H

#include "node/list.h"
#include "node/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_SECTION(ARGS...) \
    new Language::Section(Node::context()->child("Object", "Language", "Section"), ##ARGS)

    class Section : public GenericList<Primitive *> {
        CHILD_DECLARE(Section, List);
    public:
        explicit Section(Node *origin, Primitive *label = NULL) :
            GenericList<Primitive *>(origin), _label(label) {}

        CHILD_FORK_METHOD(Section, CHILD_FORK_IF_NOT_NULL(label()));

        Primitive *label() const { return _label; }
        void setLabel(Primitive *label) { _label = label; }

        virtual Node *run(Node *receiver = context());

        virtual QString toString(bool debug = false, short level = 0) const;
    private:
        Primitive *_label;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_SECTION_H
