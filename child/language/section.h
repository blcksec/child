#ifndef CHILD_LANGUAGE_SECTION_H
#define CHILD_LANGUAGE_SECTION_H

#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(Section, List);

    #define CHILD_SECTION(ARGS...) \
    Language::SectionPointer(new Language::Section(Node::context()->child("Object", "Language", "Section"), ##ARGS))

    class Section : public GenericList<SectionPointer, PrimitiveChainPointer> {
        CHILD_DECLARE(Section, List);
    public:
        explicit Section(const Pointer &origin) : GenericList<SectionPointer, PrimitiveChainPointer>(origin) {}

        static void initRoot() { Language::root()->addChild("Section", root()); }

        virtual Pointer fork() const {
            SectionPointer forkedSection = new Section(this);
            forkedSection->initFork();
            forkedSection->setLabel(label());
            return forkedSection;
        }

        PrimitiveChainPointer label() const { return _label; }
        void setLabel(const PrimitiveChainPointer &label) { _label = label; }

        virtual Pointer run(const Pointer &receiver = context()) {
            Pointer result;
            Iterator i(this);
            while(PrimitiveChainPointer chain = i.next()) {
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
        PrimitiveChainPointer _label;
    };

    CHILD_POINTER_DEFINE(Section, List);
}

CHILD_END

#endif // CHILD_LANGUAGE_SECTION_H
