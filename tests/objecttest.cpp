#include "tests/objecttest.h"

namespace Child {
    void ObjectTest::initialize() {
        QVERIFY(Module::root()->hasDirectChild("Object"));
        QVERIFY(Module::root()->child("Object")->isBasedOn(Module::root()));
    }
}
