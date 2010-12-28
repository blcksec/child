#include "tests/listtest.h"

CHILD_BEGIN

void ListTest::initialize() {
    QVERIFY(List::root()->origin() == Object::root());
    QVERIFY(Object::root()->child("List") == List::root());
}

void ListTest::insertGetAndSet() { // TODO: more tests with fork
    List *list = CHILD_LIST();
    QCOMPARE(list->size(), 0);
    QVERIFY(list->isEmpty());
    QVERIFY(!list->isNotEmpty());
    QCATCH(list->get(-1), IndexOutOfBoundsException);
    QCATCH(list->get(0), IndexOutOfBoundsException);
    Text *t1 = CHILD_TEXT("val1");
    QCATCH(list->set(-1, t1), IndexOutOfBoundsException);
    QCATCH(list->set(0, t1), IndexOutOfBoundsException);
    QCATCH(list->insert(-1, t1), IndexOutOfBoundsException);
    QCATCH(list->insert(1, t1), IndexOutOfBoundsException);
    list->insert(0, t1);
    QCOMPARE(list->size(), 1);
    QVERIFY(list->isNotEmpty());
    QVERIFY(list->get(0) == t1);
    QVERIFY(t1->hasDirectParent(list));
    Text *t2 = CHILD_TEXT("val2");
    list->set(0, t2);
    QCOMPARE(list->size(), 1);
    QVERIFY(list->get(0) == t2);
    QVERIFY(!t1->hasDirectParent(list));
    QVERIFY(t2->hasDirectParent(list));
    Text *t3 = CHILD_TEXT("val3");
    list->insert(0, t3);
    QCOMPARE(list->size(), 2);
    QVERIFY(list->get(0) == t3);
    QVERIFY(list->get(1) == t2);

    List *newList = list->fork();
    QCOMPARE(newList->size(), 2);
    QVERIFY(newList->get(0)->origin() == t3);
    QVERIFY(newList->get(1)->origin() == t2);
    QVERIFY(newList->get(0)->hasDirectParent(newList));
    QVERIFY(newList->get(1)->hasDirectParent(newList));
}

void ListTest::remove() { // TODO: test with fork
    List *list = CHILD_LIST();
    Text *t1 = CHILD_TEXT("val1");
    Text *t2 = CHILD_TEXT("val2");
    Text *t3 = CHILD_TEXT("val3");
    list->append(t1);
    list->append(t2);
    list->append(t3);
    QCOMPARE(list->size(), 3);
    QVERIFY(list->get(0) == t1);
    QVERIFY(t1->hasDirectParent(list));
    QVERIFY(list->get(1) == t2);
    QVERIFY(list->get(2) == t3);
    QCATCH(list->remove(-1), IndexOutOfBoundsException);
    QCATCH(list->remove(3), IndexOutOfBoundsException);
    list->remove(0);
    QCOMPARE(list->size(), 2);
    QVERIFY(!t1->hasDirectParent(list));
    QVERIFY(list->get(0) == t2);
    QVERIFY(list->get(1) == t3);
    list->remove(1);
    QCOMPARE(list->size(), 1);
    QVERIFY(list->get(0) == t2);
    list->remove(0);
    QCOMPARE(list->size(), 0);
}

void ListTest::clear() {
    List *list = CHILD_LIST();
    Text *t1 = CHILD_TEXT("val1");
    Text *t2 = CHILD_TEXT("val2");
    Text *t3 = CHILD_TEXT("val3");
    list->append(t1);
    list->append(t2);
    list->append(t3);
    QCOMPARE(list->size(), 3);
    QVERIFY(t1->hasDirectParent(list));
    QVERIFY(t2->hasDirectParent(list));
    QVERIFY(t3->hasDirectParent(list));
    list->clear();
    QCOMPARE(list->size(), 0);
    QVERIFY(!t1->hasDirectParent(list));
    QVERIFY(!t2->hasDirectParent(list));
    QVERIFY(!t3->hasDirectParent(list));
}

CHILD_END
