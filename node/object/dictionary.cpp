#include "node/object/dictionary.h"
#include "node/object/message.h"
#include "node/object/text.h"

CHILD_BEGIN

CHILD_DEFINE(Dictionary, Object, Object);

void Dictionary::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Dictionary, init);

    CHILD_ADD_NATIVE_METHOD(Dictionary, get, []);

    CHILD_ADD_NATIVE_METHOD(Dictionary, size);
    CHILD_ADD_NATIVE_METHOD(Dictionary, empty_qm, empty?);
}

CHILD_DEFINE_NATIVE_METHOD(Dictionary, init) {
    CHILD_FIND_LAST_MESSAGE;
    for(int i = 0; i < message->numInputs(); ++i) {
        Node *key = NULL;
        Primitive *primitive = message->input(i)->label();
        if(!primitive) CHILD_THROW(ArgumentException, "missing key in Dictionary initialization");
        if(primitive->hasNext()) CHILD_THROW(ArgumentException, "invalid key in Dictionary initialization");
        Message *msg = Message::dynamicCast(primitive->value());
        if(msg) {
            if(msg->inputs(false) || msg->outputs(false) || msg->isEscaped() || msg->isParented()
                    || msg->isVariadic() || msg->hasCodeInput())
                CHILD_THROW(ArgumentException, "invalid key in Dictionary initialization");
            key = CHILD_TEXT(msg->name());
        } else
            key = primitive->run();
        set(key, message->runInput(i));
    }
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Dictionary, get) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return get(message->runFirstInput());
}

CHILD_DEFINE_NATIVE_METHOD(Dictionary, size) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(size());
}

CHILD_DEFINE_NATIVE_METHOD(Dictionary, empty_qm) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(isEmpty());
}

CHILD_END
