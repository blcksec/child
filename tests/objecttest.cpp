#include "tests/objecttest.h"

namespace Child {

void ObjectTest::initialize() {
    QCOMPARE(Object::root()->origin(), Node::root());
    QCOMPARE(ObjectPtr(Node::root()->child("Object")), Object::root());
}

} // namespace Child
