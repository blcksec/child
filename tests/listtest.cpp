#include "tests/listtest.h"

namespace Child {

void ListTest::initialize() {
    QVERIFY(List::root()->origin() == Object::root());
    QVERIFY(ListPtr(Object::root()->child("List")) == List::root());
}

void ListTest::insertGetAndSet() { // TODO: more tests with fork
    ListPtr list(new List());
    QCOMPARE(list->size(), 0);
    QVERIFY(list->isEmpty());
    QVERIFY(!list->isNotEmpty());
    QCATCH(list->get(-1), IndexOutOfBoundsException);
    QCATCH(list->get(0), IndexOutOfBoundsException);
    TextPtr t1(new Text("val1"));
    QCATCH(list->set(-1, t1), IndexOutOfBoundsException);
    QCATCH(list->set(0, t1), IndexOutOfBoundsException);
    QCATCH(list->insert(-1, t1), IndexOutOfBoundsException);
    QCATCH(list->insert(1, t1), IndexOutOfBoundsException);
    list->insert(0, t1);
    QCOMPARE(list->size(), 1);
    QVERIFY(list->isNotEmpty());
    QCOMPARE(TextPtr(list->get(0)), t1);
    QVERIFY(t1->hasDirectParent(list));
    TextPtr t2(new Text("val2"));
    list->set(0, t2);
    QCOMPARE(list->size(), 1);
    QCOMPARE(TextPtr(list->get(0)), t2);
    QVERIFY(!t1->hasDirectParent(list));
    QVERIFY(t2->hasDirectParent(list));
    TextPtr t3(new Text("val3"));
    list->insert(0, t3);
    QCOMPARE(list->size(), 2);
    QCOMPARE(TextPtr(list->get(0)), t3);
    QCOMPARE(TextPtr(list->get(1)), t2);

    ListPtr newList = list->fork();
    QCOMPARE(newList->size(), 2);
    QCOMPARE(TextPtr(newList->get(0)->origin()), t3);
    QCOMPARE(TextPtr(newList->get(1)->origin()), t2);
    QVERIFY(newList->get(0)->hasDirectParent(newList));
    QVERIFY(newList->get(1)->hasDirectParent(newList));
}

void ListTest::remove() { // TODO: test with fork
    ListPtr list(new List());
    TextPtr t1(new Text("val1"));
    TextPtr t2(new Text("val2"));
    TextPtr t3(new Text("val3"));
    list->append(t1);
    list->append(t2);
    list->append(t3);
    QCOMPARE(list->size(), 3);
    QCOMPARE(TextPtr(list->get(0)), t1);
    QVERIFY(t1->hasDirectParent(list));
    QCOMPARE(TextPtr(list->get(1)), t2);
    QCOMPARE(TextPtr(list->get(2)), t3);
    QCATCH(list->remove(-1), IndexOutOfBoundsException);
    QCATCH(list->remove(3), IndexOutOfBoundsException);
    list->remove(0);
    QCOMPARE(list->size(), 2);
    QVERIFY(!t1->hasDirectParent(list));
    QCOMPARE(TextPtr(list->get(0)), t2);
    QCOMPARE(TextPtr(list->get(1)), t3);
    list->remove(1);
    QCOMPARE(list->size(), 1);
    QCOMPARE(TextPtr(list->get(0)), t2);
    list->remove(0);
    QCOMPARE(list->size(), 0);
}

void ListTest::clear() {
    ListPtr list(new List());
    TextPtr t1(new Text("val1"));
    TextPtr t2(new Text("val2"));
    TextPtr t3(new Text("val3"));
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

} // namespace Child
