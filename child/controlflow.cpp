#include "child/controlflow.h"
#include "child/boolean.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(ControlFlow, Node);

CHILD_DEFINE(ControlFlow::If, Node);

Node *ControlFlow::If::receive(Primitive *primitive) {
    Node *testResult = _test->run();
    if(testResult->toBool() == _isIf) return primitive->run(_receiver);
    Block *block = Block::dynamicCast(primitive->value());
    if(block && block->elseSection()) return block->elseSection()->run(_receiver);
    throw Skip(testResult);
}

Node *ControlFlow::If::ifOrUnless(Node *receiver, Message *message, bool isIf) {
    CHILD_CHECK_INPUT_SIZE(1, 3);
    if(message->hasASecondInput()) {
        Node *testResult = message->runFirstInput();
        if(testResult->toBool() == isIf) return message->runSecondInput(receiver);
        if(message->hasAThirdInput()) return message->runThirdInput(receiver);
        return testResult;
    }
    return CHILD_IF(receiver, message->firstInput()->value(), isIf);
}

//Node *Object::ifOrUnless(Message *message, bool isIf) {
//    CHILD_CHECK_INPUT_SIZE(1, 3);
//    Node *result = message->runFirstInput();
//    if(result->toBool() == isIf)
//        return message->runInputOrSection(1, "body", result);
//    else
//        return message->runInputOrSection(2, "else", result);
//}

CHILD_END
