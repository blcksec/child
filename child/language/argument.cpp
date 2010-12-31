#include "child/message.h"
#include "child/language/argument.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Argument, Pair);
    CHILD_DEFINE(ArgumentBunch, Bunch);

    QString Argument::labelName() const {
        if(label()->hasNext()) CHILD_THROW(ArgumentException, "illegal parameter label");
        Message *labelMessage = Message::dynamicCast(label()->value());
        if(!labelMessage) CHILD_THROW(ArgumentException, "illegal parameter label");
        return labelMessage->name();
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
