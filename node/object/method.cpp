#include "node/object/method.h"

CHILD_BEGIN

CHILD_DEFINE(Method, Element, Object);

void Method::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Method, init);

    CHILD_ADD_NATIVE_METHOD(Method, return);
}

CHILD_DEFINE_NATIVE_METHOD(Method, init) {
    CHILD_FIND_LAST_MESSAGE;
    if(message->hasAnInput()) {
        Primitive *primitive = Primitive::dynamicCast(message->runFirstInput());
        if(!primitive) CHILD_THROW(ArgumentException, "code parameter must be a Primitive");
        setCode(primitive);
        if(message->hasASecondInput()) {
            List *list = List::dynamicCast(message->runSecondInput());
            if(!list) CHILD_THROW(ArgumentException, "inputs parameter must be a List");
//            setInputs(list);
        }
    } else {
        CHILD_FIND_LAST_PRIMITIVE;
        Primitive *nextPrimitive = primitive->next();
        if(nextPrimitive) {
            setCode(nextPrimitive);
            Primitive::skip(this);
        }
    }
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Method, return) {
    CHILD_FIND_LAST_PRIMITIVE;
    if(primitive->next())
        CHILD_THROW(InterpreterException, "dead code found after a return statement");
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasInput(0) ? message->runFirstInput() : CHILD_NODE();
    throw Return(result);
}

CHILD_END
