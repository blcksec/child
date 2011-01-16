#include "node/object/list.h"
#include "node/object/number.h"

CHILD_BEGIN

CHILD_DEFINE(AbstractList, Object, Object);

void AbstractList::initRoot() {
    CHILD_ADD_NATIVE_METHOD(AbstractList, get, []);

    CHILD_ADD_NATIVE_METHOD(AbstractList, append);

    CHILD_ADD_NATIVE_METHOD(AbstractList, size);
    CHILD_ADD_NATIVE_METHOD(AbstractList, empty_qm, empty?);
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, get) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    int index = message->runFirstInput()->toDouble();
    return get(index);
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, append) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    Node *value = message->runFirstInput();
    append(value);
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, size) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(size());
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, empty_qm) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(isEmpty());
}

CHILD_DEFINE(List, AbstractList, Object);

void List::initRoot() {
    CHILD_ADD_NATIVE_METHOD(List, init);
}

CHILD_DEFINE_NATIVE_METHOD(List, init) {
    CHILD_FIND_LAST_MESSAGE;
    for(int i = 0; i < message->numInputs(); ++i)
        append(message->runInput(i));
    return this;
}

CHILD_DEFINE(VirtualList, AbstractList, Object);

void VirtualList::initRoot() {
}

CHILD_END
