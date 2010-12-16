#include "child/object.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Object, Node);

CHILD_NATIVE_METHOD_DEFINE(Object, less_than) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() < 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, less_than_or_equal_to) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() <= 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() > 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than_or_equal_to) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() >= 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, if) {
    CHILD_CHECK_INPUT_SIZE(1, 3);
    Pointer result = message->runFirstInput();
    bool isTrue = result->toBool();
    if(message->inputs()->size() >= 2) {
        if(message->block())
            CHILD_THROW(ArgumentException, "an 'if' statement cannot have both a second argument and a block");
        if(isTrue)
            return message->runSecondInput();
        else if(message->inputs()->size() == 3)
            return message->runThirdInput();
    } else if(message->block()) {
        if(isTrue) {
            if(message->block()->bodySection()->isNotEmpty())
                return message->block()->bodySection()->run();
        } else if(message->block()->elseSection()->isNotEmpty())
            return message->block()->elseSection()->run();
    }
    return result;
}

CHILD_END
