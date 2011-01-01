#include "child/controlflow.h"
#include "child/boolean.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(ControlFlow, Node);

CHILD_NATIVE_METHOD_DEFINE(ControlFlow, break) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasAnInput() ? message->runFirstInput() : CHILD_NODE();
    throw Break(result);
}

// === If ===

CHILD_DEFINE(ControlFlow::If, Node);

Node *ControlFlow::If::make(Node *receiver, Message *message, bool isIf) {
    CHILD_CHECK_INPUT_SIZE(1, 3);
    if(message->hasASecondInput()) {
        Node *testResult = message->runFirstInput();
        if(testResult->toBool() == isIf) return message->runSecondInput(receiver);
        if(message->hasAThirdInput()) return message->runThirdInput(receiver);
        return testResult;
    }
    return CHILD_IF(receiver, message->firstInput()->value(), isIf);
}

Node *ControlFlow::If::receive(Primitive *primitive) {
    Node *testResult = _test->run();
    if(testResult->toBool() == _isIf) return primitive->run(_receiver);
    Block *block = Block::dynamicCast(primitive->value());
    if(block && block->elseSection()) return block->elseSection()->run(_receiver);
    return testResult;
}

// === Loop ===

CHILD_DEFINE(ControlFlow::Loop, Node);

Node *ControlFlow::Loop::make(Node *receiver, Message *message) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    HugeInteger count;
    if(message->hasAnInput()) { // finite loop
        count = message->runFirstInput()->toDouble();
        if(count < 0) CHILD_THROW(ArgumentException, "loop count must be greater (or equal) than 0");
    } else
        count = -1; // infinite loop
    return CHILD_LOOP(receiver, count);
}

Node *ControlFlow::Loop::receive(Primitive *primitive) {
    Node *result = NULL;
    try {
        if(_count > 0)
            for(HugeInteger i = 0; i < _count; ++i)
                result = primitive->run(_receiver);
        else if (_count == 0)
            result = CHILD_NODE();
        else
            while(true) primitive->run(_receiver);
    } catch(const Break &brk) {
        result = brk.result;
    }
    return result;
}

CHILD_END
