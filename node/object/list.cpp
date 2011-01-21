#include "node/object/list.h"
#include "node/object/number.h"

CHILD_BEGIN

// === AbastractList ===

CHILD_DEFINE(AbstractList, Object, Object);

void AbstractList::initRoot() {
    CHILD_ADD_NATIVE_METHOD(AbstractList, get, []);
    CHILD_ADD_NATIVE_METHOD(AbstractList, set, []=);
    CHILD_ADD_NATIVE_METHOD(AbstractList, append_or_set, []:=);

    CHILD_ADD_NATIVE_METHOD(AbstractList, append);
    CHILD_ADD_NATIVE_METHOD(AbstractList, remove, []>>);

    CHILD_ADD_NATIVE_METHOD(AbstractList, size);
    CHILD_ADD_NATIVE_METHOD(AbstractList, empty);
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, get) { // TODO: use multiple return values (-> index, found)
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    Node *value = NULL;
    if(Primitive *label = message->firstInput()->label()) {
        Message *msg = Message::dynamicCast(label->value());
        if(msg && msg->name() == "value")
            value = message->runFirstInput();
    }
    bool wasFound = true;
    if(value) {
        int index = get(&value, message->isQuestioned() ? &wasFound : NULL);
        if(wasFound) return CHILD_NUMBER(index);
    } else {
        int index = message->runFirstInput()->toDouble();
        value = get(index, message->isQuestioned() ? &wasFound : NULL);
        if(wasFound) return value;
    }
    Primitive::skip(CHILD_BOOLEAN(false));
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, set) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    int index = message->runFirstInput()->toDouble();
    return set(index, message->runSecondInput());
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, append_or_set) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1, 2);
    int index;
    if(message->hasASecondInput())
        index = message->runFirstInput()->toDouble();
    else
        index = size();
    Node *value = message->runLastInput();
    if(hasIndex(index))
        return set(index, value);
    else {
        int numBlanksToAppend = index - size();
        for(int i = 1; i <= numBlanksToAppend; ++i) append(CHILD_NODE());
        return append(value);
    }
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, append) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    Node *value = message->runFirstInput();
    append(value);
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, remove) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    Node *value = NULL;
    if(Primitive *label = message->firstInput()->label()) { // TODO: DRY!
        Message *msg = Message::dynamicCast(label->value());
        if(msg && msg->name() == "value")
            value = message->runFirstInput();
    }
    int index;
    bool wasFound = true;
    if(value) {
        index = get(&value, message->isQuestioned() ? &wasFound : NULL);
    } else {
        index = message->runFirstInput()->toDouble();
        value = get(index, message->isQuestioned() ? &wasFound : NULL);
    }
    if(!wasFound) Primitive::skip(CHILD_BOOLEAN(false));
    remove(index);
    return value;
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, size) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(size());
}

CHILD_DEFINE_NATIVE_METHOD(AbstractList, empty) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(isEmpty());
}

// === List ===

CHILD_DEFINE(List, AbstractList, Object);

void List::initRoot() {
    CHILD_ADD_NATIVE_METHOD(List, init);
}

CHILD_DEFINE_NATIVE_METHOD(List, init) {
    CHILD_FIND_LAST_MESSAGE;
    for(int i = 0; i < message->numInputs(); ++i)
        append(message->runInput(i));

    // === TODO: DRY ===
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(nextPrimitive) {
        nextPrimitive->run(this);
        Primitive::skip(this);
    }

    return this;
}

CHILD_DEFINE(VirtualList, AbstractList, Object);

void VirtualList::initRoot() {
}

CHILD_END
