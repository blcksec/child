#include "child/language/parameter.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Parameter, Pair);

    void Parameter::initRoot() {
        Language::root()->addChild("Parameter", root());
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

    CHILD_DEFINE(ParameterList, List);

    void ParameterList::initRoot() {
        Language::root()->addChild("ParameterList", root());
    }

    void ParameterList::hasChanged() {
        _labels.clear();
        Iterator i(this);
        while(Parameter *parameter = i.next()) {
            if(_labels.contains(parameter->label()))
                CHILD_THROW(DuplicateException, "duplicated label found in parameter list");
            _labels.insert(parameter->label(), parameter);
        }
    }
}

CHILD_END
