#ifndef CHILD_METHOD_H
#define CHILD_METHOD_H

#include "child/element.h"
#include "child/block.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Method, Element);

#define CHILD_METHOD(ARGS...) MethodPointer(new Method(Node::context()->child("Object", "Method"), ##ARGS))

class Method : public GenericElement<MethodPointer, BlockPointer> {
    CHILD_DECLARE(Method, Element);
public:
    Method(const Pointer &origin, const BlockPointer value = BlockPointer::null()) :
        GenericElement<MethodPointer, BlockPointer>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Method", root());
    }

    virtual Pointer fork() const { return new Method(this, value()->fork()); }
};

CHILD_POINTER_DEFINE(Method, Element);

CHILD_END

#endif // CHILD_METHOD_H
