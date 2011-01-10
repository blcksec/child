#include "child/language/section.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Section, List);

    void Section::initRoot() {
        Language::root()->addChild("Section", this);
    }

    Node *Section::run(Node *receiver) {
        Node *result = NULL;
        Iterator i(this);
        while(Primitive *primitive = i.next())
            result = receiver->receive(primitive);
        return result;
    }

    QString Section::toString(bool debug, short level) const {
        QString str;
        if(label()) str += QString("    ").repeated(level - 1) + label()->toString(debug, level) + ":";
        str = concatenateStrings(str, "\n", join("\n", QString("    ").repeated(level), "", debug, level));
        return str;
    }
}

CHILD_END
