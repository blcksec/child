#include "tests/objecttest.h"

CHILD_BEGIN

void ObjectTest::initialize() {
    QCOMPARE(Object::root()->origin(), Node::root());
    QCOMPARE(Node::root()->child("Object"), Object::root());
}

CHILD_END
