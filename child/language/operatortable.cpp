#include "child/language/operatortable.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(OperatorTable, List);

    void OperatorTable::initRoot() {
        Language::root()->addChild("OperatorTable", root());
    }
}

CHILD_END
