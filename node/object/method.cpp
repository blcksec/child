#include "node/object/method.h"

CHILD_BEGIN

CHILD_DEFINE(Method, Element, Object);

void Method::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Method, init);
}

CHILD_DEFINE_NATIVE_METHOD(Method, init) {
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(nextPrimitive) {
        Block *block = Block::dynamicCast(nextPrimitive->value());
        if(block) {
            setBlock(block);
            throw Primitive::Skip(this);
        }
    }
    return this;
}

CHILD_END
