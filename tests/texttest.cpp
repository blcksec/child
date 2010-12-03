#include "tests/texttest.h"

namespace Child {

void TextTest::initialize() {
    QVERIFY(Text::root()->origin() == Object::root());
    QVERIFY(TextPtr(Object::root()->child("Text")) == Text::root());
}

void TextTest::fork() {
    TextPtr text(new Text("Hello"));
    QVERIFY(text->origin() == Text::root());
    QCOMPARE(text->value(), QString("Hello"));
    TextPtr newText(text->fork());
    QVERIFY(newText->origin() == text);
    QCOMPARE(newText->value(), QString("Hello"));
}

void TextTest::compare() {
    TextPtr t1(new Text("Hello"));
    TextPtr t2(new Text("Hello"));
    TextPtr t3(new Text("HELLO"));
    NodePtr n(new Node());
    QVERIFY(t1->compare(t2) == Node::Equal);
    QVERIFY(t1->compare(t3) != Node::Equal);
//    QVERIFY(t1->compare(n) != Node::Equal); <-- You are here

//    QCOMPARE(*t1, *t2);
//    QVERIFY(*t1 == *t2);
//    QVERIFY(!(*t1 != *t2));
//    QVERIFY(*t1 != *t3);
//    Text *t4 = Text::fork(world, "Jean");
//    Text *t5 = Text::fork(world, "Pierre");
//    QVERIFY(*t4 != *t5);
//    QCOMPARE(t4->compare(t5), Node::Smaller);
//    QCOMPARE(t5->compare(t4), Node::Greater);

//    NodeRef r1(t1);
//    NodeRef r2(t2);
//    NodeRef r3(t3);
//    QCOMPARE(r1, r2);
//    QVERIFY(r1 == r2);
//    QVERIFY(!(r1 != r2));
//    QVERIFY(r1 != r3);
}

void TextTest::upcase() {
    TextPtr text(new Text("Hello"));
    QCOMPARE(text->upcase()->value(), QString("HELLO"));
}

} // namespace Child

