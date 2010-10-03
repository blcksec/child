#include "tests/objecttest.h"

namespace Child {
    void ObjectTest::initialize() {
        QVERIFY(Node::root()->hasDirectChild("Object"));
        QVERIFY(Node::root()->child("Object")->directOriginIs(Node::root()));
    }
}
