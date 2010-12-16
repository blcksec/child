#include "tests/texttest.h"

CHILD_BEGIN

void TextTest::initialize() {
    QVERIFY(Text::root()->origin() == Element::root());
    QVERIFY(Object::root()->child("Text") == Text::root());
}

void TextTest::fork() {
    TextPointer text = CHILD_TEXT("Hello");
    QVERIFY(text->origin() == Text::root());
    QCOMPARE(text->value(), QString("Hello"));
    TextPointer newText(text->fork());
    QVERIFY(newText->origin() == text);
    QCOMPARE(newText->value(), QString("Hello"));
}

void TextTest::compare() {
    TextPointer t1 = CHILD_TEXT("Hello");
    TextPointer t2 = CHILD_TEXT("Hello");
    TextPointer t3 = CHILD_TEXT("Salut");
    Pointer n = CHILD_NODE();
    QVERIFY(t1->compare(t2) == 0);
    QVERIFY(t1->compare(t3) != 0);
    QVERIFY(t1->compare(n) != 0);
    QVERIFY(t1 != t2);
    QCOMPARE(*t1, *t2);
    QVERIFY(*t1 == *t2);
    QVERIFY(!(*t1 != *t2));
    QVERIFY(*t1 != *t3);
    TextPointer t4 = CHILD_TEXT("Jean");
    TextPointer t5 = CHILD_TEXT("Pierre");
    QVERIFY(*t4 != *t5);
    QCOMPARE(t4->compare(t5), short(-1));
    QCOMPARE(t5->compare(t4), short(1));
}

CHILD_END
