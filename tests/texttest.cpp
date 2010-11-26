#include "tests/texttest.h"

namespace Child {
    void TextTest::initialize() {
        QVERIFY(Node::root()->child("Object")->hasDirectChild("Text"));
    }

    void TextTest::fork() {
        World w; World *world = &w;
        Text *t1 = Text::fork(world, "Hello");
        QVERIFY(t1->directOriginIs(world->child("Object")->child("Text"))); // FIXME: should work with world->child("Text") too?
        QCOMPARE(t1->value(), QString("Hello"));
        QCOMPARE(Text::as(t1->child("upcase"))->value(), QString("HELLO"));
        Text *t2 = t1->fork();
        QVERIFY(t2->directOriginIs(t1));
        QCOMPARE(t2->value(), QString("Hello"));
    }

    void TextTest::compare() {
        World w; World *world = &w;
        Text *t1 = Text::fork(world, "Hello");
        Text *t2 = Text::fork(world, "Hello");
        Text *t3 = Text::fork(world, "HELLO");
        Node *n = Node::fork(world);
        QVERIFY(t1->compare(t2) == Node::Equal);
        QVERIFY(t1->compare(t3) != Node::Equal);
        QVERIFY(t1->compare(n) != Node::Equal);
        QCOMPARE(*t1, *t2);
        QVERIFY(*t1 == *t2);
        QVERIFY(!(*t1 != *t2));
        QVERIFY(*t1 != *t3);
        Text *t4 = Text::fork(world, "Jean");
        Text *t5 = Text::fork(world, "Pierre");
        QVERIFY(*t4 != *t5);
        QCOMPARE(t4->compare(t5), Node::Smaller);
        QCOMPARE(t5->compare(t4), Node::Greater);

        NodeRef r1(t1);
        NodeRef r2(t2);
        NodeRef r3(t3);
        QCOMPARE(r1, r2);
        QVERIFY(r1 == r2);
        QVERIFY(!(r1 != r2));
        QVERIFY(r1 != r3);
    }
}

