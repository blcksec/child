#ifndef CHILD_LANGUAGE_SECTION_H
#define CHILD_LANGUAGE_SECTION_H

#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_SECTION(ARGS...) \
    new Language::Section(Node::context()->child("Object", "Language", "Section"), ##ARGS)

    class Section : public GenericList<PrimitiveChain *> {
        CHILD_DECLARE(Section, List);
    public:
        explicit Section(Node *origin, PrimitiveChain *label = NULL) :
            GenericList<PrimitiveChain *>(origin), _label(label) {}

        static void initRoot() { Language::root()->addChild("Section", root()); }

        CHILD_FORK_METHOD(Section, CHILD_FORK_IF_NOT_NULL(label()));

        PrimitiveChain *label() const { return _label; }
        void setLabel(PrimitiveChain *label) { _label = label; }

        virtual Node *run(Node *receiver = context()) {
            Node *result = NULL;
            Iterator i(this);
            while(PrimitiveChain *chain = i.next()) {
                result = chain->run(receiver);
            }
            return result;
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            QString str;
            if(label() && label()->isNotEmpty()) str += QString("    ").repeated(level - 1) + label()->toString(debug, level) + ":";
            str = concatenateStrings(str, "\n", join("\n", QString("    ").repeated(level), "", debug, level));
            return str;
        }
    private:
        PrimitiveChain *_label;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_SECTION_H
