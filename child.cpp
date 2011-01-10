#include "child.h"
#include "child/node.h"
#include "child/language/interpreter.h"

CHILD_BEGIN

void init() {
    Interpreter *interpreter = Interpreter::root();
    QString path = "../child/child";
    foreach(Node *root, Node::roots()) {
        QString childFile = path + "/" + root->className().toLower() + ".child";
        if(QFileInfo(childFile).exists()) {
            interpreter->loadSourceCode(childFile)->run();
        }
    }
    interpreter->testSuite()->run();
    P(QString("All tests passed (%1 sections, %2 assertions)").
      arg(interpreter->testSuite()->size()).arg(Node::passedAssertionCount()));
}

CHILD_END
