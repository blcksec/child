#include "tests/objecttest.h"

CHILD_BEGIN

void ObjectTest::initialize() {
    QVERIFY(Object::root()->origin() == Node::root());
    QVERIFY(Node::root()->child("Object") == Object::root());
}

CHILD_END
