#include "node/object/list.h"
#include "node/object/number.h"

CHILD_BEGIN

CHILD_DEFINE(AbstractList, Object, Object);

void AbstractList::initRoot() {
    CHILD_NATIVE_METHOD_ADD(AbstractList, append);
    CHILD_NATIVE_METHOD_ADD(AbstractList, append, <<);

    CHILD_NATIVE_METHOD_ADD(AbstractList, size);
    CHILD_NATIVE_METHOD_ADD(AbstractList, empty_qm, empty?);
}

CHILD_NATIVE_METHOD_DEFINE(AbstractList, append) {
    CHILD_CHECK_INPUT_SIZE(1);
    Node *value = message->runFirstInput();
    append(value);
    return value;
}

CHILD_NATIVE_METHOD_DEFINE(AbstractList, size) {
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(size());
}

CHILD_NATIVE_METHOD_DEFINE(AbstractList, empty_qm) {
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(isEmpty());
}

CHILD_DEFINE(List, AbstractList, Object);

void List::initRoot() {
}

CHILD_DEFINE(VirtualList, AbstractList, Object);

void VirtualList::initRoot() {
}

CHILD_END
