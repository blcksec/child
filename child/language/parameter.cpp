#include "child/language/parameter.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Parameter, Pair);

    void Parameter::initRoot() {
        Language::root()->addChild("Parameter", root());
    }

    CHILD_DEFINE(ParameterList, List);

    void ParameterList::initRoot() {
        Language::root()->addChild("ParameterList", root());
    }
}

CHILD_END
