#include "tests/texttest.h"

namespace Child {
    void TextTest::initialize() {
        QVERIFY(Module::root()->child("Object")->hasDirectChild("Text"));
    }

    void TextTest::fork() {
        World w; World *world = &w;
        Text *t1 = CHILD_TEXT(world->child("Text"))->fork("Hello");
        QVERIFY(t1->isBasedOn(world->child("Text")));
        QCOMPARE(t1->value(), QString("Hello"));
        QCOMPARE(CHILD_TEXT(t1->child("upcase"))->value(), QString("HELLO"));
        Text *t2 = t1->fork();
        QVERIFY(t2->isBasedOn(t1));
        QCOMPARE(t2->value(), QString("Hello"));
    }
}

