#include "node/object/language/parameter.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Parameter, Pair, Language);

    void Parameter::initRoot() {
    }

    QString Parameter::toString(bool debug, short level) const {
        QString prefixes;
        if(isEscaped())
            prefixes += "\\";
        if(isParented())
            prefixes += "@";
        return concatenateStrings(prefixes + label(), ": ",
                                  defaultValue() ? defaultValue()->toString(debug, level) : "");
    }
}

CHILD_END
