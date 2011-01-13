#include "node/object/language/parameterlist.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(ParameterList, List, Language);

    void ParameterList::initRoot() {
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
