#include "node/controlflow.h"
#include "node/object/boolean.h"
#include "node/object/message.h"
#include "node/object/language/block.h"

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
        if(testResult->toBool() == isIf) Primitive::skip(nextPrimitive->run(this));
        Block *block = Block::dynamicCast(nextPrimitive->value());
        if(block && block->elseSection()) Primitive::skip(block->elseSection()->run(this));
        Primitive::skip(testResult);
    }
    CHILD_THROW(InterpreterException, QString("missing code after an %1 statement").arg(isIf ? "if" : "unless"));
}

Node *ControlFlow::loop(LoopType type) {
    CHILD_FIND_LAST_MESSAGE;
    HugeInteger count;
    Primitive *testCode;
    if(type == Loop) {
        CHILD_CHECK_INPUT_SIZE(0, 2);
        if(message->hasAnInput()) {
            count = message->runFirstInput()->toDouble();
            if(count < -1) CHILD_THROW(ArgumentException, "loop count must be -1, 0 or greater");
        } else
            count = -1; // infinite loop
    } else {
        CHILD_CHECK_INPUT_SIZE(1, 2);
        testCode = message->firstInput()->value();
    }
    Primitive *primaryCode;
    if(message->hasASecondInput()) {
        primaryCode = Primitive::dynamicCast(message->runSecondInput());
        if(!primaryCode) CHILD_THROW(ArgumentException, "'primary_code' parameter must be a Primitive");
    } else
        primaryCode = NULL;
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *secondaryCode = primitive->next();
    if(!primaryCode && secondaryCode) {
        primaryCode = secondaryCode;
        secondaryCode = NULL;
    }
    if(!primaryCode)
        CHILD_THROW(InterpreterException, "missing code after loop statement");

    Section *before = NULL;
    Primitive *body = primaryCode;
    Section *after = NULL;
    Section *between = NULL;
    Block *block = Block::dynamicCast(primaryCode->value());
    if(block) {
        before = block->beforeSection();
        after = block->afterSection();
        between = block->betweenSection();
    }
    if(secondaryCode) {
        block = Block::dynamicCast(secondaryCode->value());
        if(block) {
            if(block->beforeSection()) before = block->beforeSection();
            if(block->bodySection()) body = secondaryCode;
            if(block->afterSection()) after = block->afterSection();
            if(block->betweenSection()) between = block->betweenSection();
        } else
            body = secondaryCode;
    }

    CHILD_PUSH_CONTEXT(findLastNativeMethod());
    Node *result = NULL;
    try {
        HugeInteger i = 0;
        Node *test = NULL;
        bool first = true;
        while(true) {
            if(type == Loop) {
                if(count >= 0 && i == count) break;
            } else if(type == While) {
                test = testCode->run();
                if(test->toBool()) result = test; else break;
            }
            try {
                if(before) before->run();
                if(!first) {
                    if(between) try { between->run(); } catch(Continue) {}
                } else
                    first = false;
                result = body->run();
            } catch(Continue) {}
            if(after) try { after->run(); } catch(Continue) {}
            if(type == Loop) {
                if(count > 0) i++;
            } else if(type == Until) {
                test = testCode->run();
                if(test->toBool()) break;
            }
        }
    } catch(const Break &brk) {
        result = brk.result;
    }
    if(!result) result = CHILD_NODE();
    Primitive::skip(result);
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

CHILD_END
