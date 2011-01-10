#include "child.h"
#include "child/node.h"
#include "child/language/interpreter.h"

CHILD_BEGIN

void init() {
    foreach(Node *root, Node::roots()) {
        QString path = "../child/child";
        QString childFile = path + "/" + root->className().toLower() + ".child";
        if(QFileInfo(childFile).exists()) {
//            P(childFile);
            SourceCode *source = Interpreter::root()->loadSourceCode(childFile);
            source->run();
        }
    }
}

CHILD_END
