#include "node/object/element.h"
#include "node/object/property.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Element, Object, Object);

void Element::initRoot() {
    Property *valueProperty = CHILD_PROPERTY();
    valueProperty->CHILD_ADD_NATIVE_METHOD(Element, value_get, get);
    valueProperty->CHILD_ADD_NATIVE_METHOD(Element, value_set, set);
    addChild("value", valueProperty);
}

CHILD_DEFINE_NATIVE_METHOD(Element, value_get) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    Node *result = cast(parent())->value();
    return result ? result : CHILD_NODE();
}

CHILD_DEFINE_NATIVE_METHOD(Element, value_set) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    cast(parent())->setValue(message->runFirstInput());
    return this;
}

CHILD_END
