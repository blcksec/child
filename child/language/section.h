#ifndef CHILD_LANGUAGE_SECTION_H
#define CHILD_LANGUAGE_SECTION_H

#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Section, List);

    #define CHILD_SECTION(ARGS...) \
    Language::SectionPtr(new Language::Section(Node::context()->child("Object", "Language", "Section"), ##ARGS))

    class Section : public GenericList<SectionPtr, PrimitiveChainPtr> {
        CHILD_DECLARATION(Section, List);
    public:
        Section(const NodePtr &origin) : GenericList<SectionPtr, PrimitiveChainPtr>(origin) {}

        static void initRoot() { Language::root()->addChild("Section", root()); }
        virtual NodePtr fork() const { return SectionPtr(new Section(this))->initFork(); }

        PrimitiveChainPtr label() const { return _label; }
        void setLabel(const PrimitiveChainPtr &label) { _label = label; }

        virtual const QString toString(bool debug = false) const {
            QString str;
            if(label() && label()->isNotEmpty()) str += label()->toString(debug) + ":\n";
            str += join("\n", "    ", "", debug);
            return str;
        }
    private:
        PrimitiveChainPtr _label;
    };

    CHILD_PTR_DEFINITION(Section, List);
}

CHILD_END

#endif // CHILD_LANGUAGE_SECTION_H
