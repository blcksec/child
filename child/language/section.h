#ifndef CHILD_SECTION_H
#define CHILD_SECTION_H

#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Section, List);

    #define CHILD_SECTION(ARGS...) \
    Language::SectionPtr(new Language::Section(Node::findInContext("Object")->child("Language")->child("Section"), ##ARGS))

    class Section : public GenericList<SectionPtr, PrimitiveChainPtr> {
        CHILD_DECLARATION(Section, List);
    public:
        Section(const NodePtr &origin) : GenericList<SectionPtr, PrimitiveChainPtr>(origin) {}

        static void initRoot() { Language::root()->addChild("Section", root()); }
        virtual NodePtr fork() const { CHILD_TODO; return new Section(this); }

        PrimitiveChainPtr label() const { return _label; }
        void setLabel(const PrimitiveChainPtr &label) { _label = label; }

        virtual const QString toString(bool debug = false) const {
            QString str;
            if(label()->isNotEmpty()) str += label()->toString(debug) + ":\n";
            str += join("\n", "    ", "", debug);
            return str;
        }
    private:
        PrimitiveChainPtr _label;
    };

    CHILD_PTR_DEFINITION(Section, List);
}

CHILD_END

#endif // CHILD_SECTION_H
