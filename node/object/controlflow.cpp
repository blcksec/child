#include "node/controlflow.h"
#include "node/boolean.h"
#include "node/message.h"
#include "node/block.h"

CHILD_BEGIN

CHILD_DEFINE(ControlFlow, Node);

void ControlFlow::initRoot() {
    Node::root()->addChild("ControlFlow", this);

    CHILD_NATIVE_METHOD_ADD(ControlFlow, if);
    CHILD_NATIVE_METHOD_ADD(ControlFlow, unless);

    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, loop);

    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, while);
    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, until);

    CHILD_NATIVE_METHOD_ADD(ControlFlow, break);
}

Node *ControlFlow::ifOrUnless(Message *message, Primitive *code, bool isIf) {
    CHILD_CHECK_INPUT_SIZE(1, 3);
    if(message->hasASecondInput()) {
        Node *testResult = message->runFirstInput();
        if(testResult->toBool() == isIf) return message->runSecondInput(this);
        if(message->hasAThirdInput()) return message->runThirdInput(this);
        return testResult;
    } else if(code) {
        Node *testResult = message->runFirstInput();
        if(testResult->toBool() == isIf) return code->run(this);
        Block *block = Block::dynamicCast(code->value());
        if(block && block->elseSection()) return block->elseSection()->run(this);
        return testResult;
    } else
        return new Message::Sending(Message::Sending::root(), message, this);
}

CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DEFINE(ControlFlow, loop) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
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
                result = code->run();
        else if (count == 0)
            result = CHILD_NODE();
        else
            while(true) code->run();
    } catch(const Break &brk) {
        result = brk.result;
    }
    return result;
}

Node *ControlFlow::whileOrUntil(Message *message, Primitive *code, bool isWhile) {
    CHILD_CHECK_INPUT_SIZE(1);
    Node *result = NULL;
    try {
        Node *test = NULL;
        while(true) {
            if(isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) result = test; else break;
            }
            result = code->run();
            if(!isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) break;
            }
        }
    } catch(const Break &brk) {
        result = brk.result;
    }
    return result;
}

CHILD_NATIVE_METHOD_DEFINE(ControlFlow, break) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasAnInput() ? message->runFirstInput() : CHILD_NODE();
    throw Break(result);
}

CHILD_END
