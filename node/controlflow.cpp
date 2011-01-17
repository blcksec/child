#include "node/controlflow.h"
#include "node/object/boolean.h"
#include "node/object/message.h"
#include "node/object/block.h"

CHILD_BEGIN

CHILD_DEFINE(ControlFlow, Node, Node);

void ControlFlow::initRoot() {
    CHILD_ADD_NATIVE_METHOD(ControlFlow, if);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, unless);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, loop);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, while);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, until);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, break);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, continue);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, thrown_qm, thrown?);
}

Node *ControlFlow::ifOrUnless(bool isIf) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1, 3);
    Node *testResult = message->runFirstInput();
    if(message->hasASecondInput()) {
        if(testResult->toBool() == isIf) return message->runSecondInput(this);
        if(message->hasAThirdInput()) return message->runThirdInput(this);
        return testResult;
    }
    CHILD_FIND_LAST_PRIMITIVE;
    if(Primitive *nextPrimitive = primitive->next()) {
        if(testResult->toBool() == isIf) throw Primitive::Skip(nextPrimitive->run(this));
        Block *block = Block::dynamicCast(nextPrimitive->value());
        if(block && block->elseSection()) throw Primitive::Skip(block->elseSection()->run(this));
        throw Primitive::Skip(testResult);
    }
    CHILD_THROW(InterpreterException, QString("missing code after an %1 statement").arg(isIf ? "if" : "unless"));
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, loop) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive)
        CHILD_THROW(InterpreterException, "missing code after a loop statement");
    HugeInteger count;
    if(message->hasAnInput()) { // finite loop
        count = message->runFirstInput()->toDouble();
        if(count < 0) CHILD_THROW(ArgumentException, "loop count must be greater (or equal) than 0");
    } else
        count = -1; // infinite loop
    Node *result = NULL;
    try {
        if(count > 0)
            for(HugeInteger i = 0; i < count; ++i)
                try { result = nextPrimitive->run(); } catch(Continue) {}
        else if (count == 0)
            result = CHILD_NODE();
        else
            while(true) try { result = nextPrimitive->run(); } catch(Continue) {}
    } catch(const Break &brk) {
        result = brk.result;
    }
    throw Primitive::Skip(result);
}

Node *ControlFlow::whileOrUntil(bool isWhile) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive)
        CHILD_THROW(InterpreterException, QString("missing code after %1 statement").arg(isWhile ? "a while" : "an until"));
    Node *result = NULL;
    try {
        Node *test = NULL;
        while(true) {
            if(isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) result = test; else break;
            }
            try { result = nextPrimitive->run(); } catch(Continue) {}
            if(!isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) break;
            }
        }
    } catch(const Break &brk) {
        result = brk.result;
    }
    throw Primitive::Skip(result);
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, break) {
    CHILD_FIND_LAST_PRIMITIVE;
    if(primitive->next())
        CHILD_THROW(InterpreterException, "dead code found after a break statement");
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasAnInput() ? message->runFirstInput() : CHILD_NODE();
    throw Break(result);
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, continue) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    throw Continue();
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, thrown_qm) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive)
        CHILD_THROW(InterpreterException, "missing code after 'thrown?' method");
    bool result = false;
    try {
        nextPrimitive->run();
    } catch(const Node &node) {
        if(node.isOriginatingFrom(this))
            result = true;
        else
            throw;
    }
    throw Primitive::Skip(CHILD_BOOLEAN(result));
}

CHILD_END
