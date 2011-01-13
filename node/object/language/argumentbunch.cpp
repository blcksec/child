#include "node/object/language/argumentbunch.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(ArgumentBunch, Bunch, Language);

    void ArgumentBunch::initRoot() {
    }

    void ArgumentBunch::append(Primitive *value) {
        if(Pair *pair = Pair::dynamicCast(value->value()))
            append(Primitive::cast(pair->first()), Primitive::cast(pair->second()));
        else if(Bunch *bunch = Bunch::dynamicCast(value->value()))
            append(bunch);
        else
            append(CHILD_ARGUMENT(NULL, value));
    }

    void ArgumentBunch::append(Primitive *label, Primitive *value) {
        append(CHILD_ARGUMENT(label, value));
    }

    void ArgumentBunch::append(const Bunch *bunch) {
        Bunch::Iterator i(bunch);
        while(Node *value = i.next()) append(Primitive::cast(value));
    }

    void ArgumentBunch::checkSpecifiedSize(short size, short min, short max) {
        if(max == -1) max = min;
        if(min == max && size != min)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (should be %1)").arg(min));
        if(size < min)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (the minimum is %1)").arg(min));
        if(size > max)
            CHILD_THROW(RuntimeException, QString("wrong number of arguments (the maximum is %1)").arg(max));
    }
}

CHILD_END
