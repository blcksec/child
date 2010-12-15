#include "child/language/argument.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINITION(Argument, Pair);
    CHILD_DEFINITION(ArgumentBunch, Bunch);

    void ArgumentBunch::checkSpecifiedSize(short size, short min, short max) {
        if(max == -1) max = min;
        if(min == max && size != min)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (should be %1)").arg(min));
        if(size < min)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (should be %1 or more)").arg(min));
        if(size > max)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (should be %1 or less)").arg(max));
    }
}

CHILD_END
