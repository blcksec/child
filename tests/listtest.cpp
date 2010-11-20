#include "tests/listtest.h"

namespace Child {
    void ListTest::initialize() {
        QVERIFY(Node::root()->child("Object")->hasDirectChild("List"));
    }

    void ListTest::insertGetAndSet() { // TODO: more tests with fork
        World w; World *world = &w;
        List *list = List::fork(world);
        QCOMPARE(list->size(), 0);
        QVERIFY(list->isEmpty());
        QVERIFY(!list->isNotEmpty());
        QCATCH(list->get(-1), IndexOutOfBoundsException);
        QCATCH(list->get(0), IndexOutOfBoundsException);
        Text *t1 = Text::fork(world, "val1");
        world->addDirectChild("t1", t1);
        QCATCH(list->set(-1, t1), IndexOutOfBoundsException);
        QCATCH(list->set(0, t1), IndexOutOfBoundsException);
        QCATCH(list->insert(-1, t1), IndexOutOfBoundsException);
        QCATCH(list->insert(1, t1), IndexOutOfBoundsException);
        list->insert(0, t1);
        QCOMPARE(list->size(), 1);
        QVERIFY(list->isNotEmpty());
        QCOMPARE(list->get(0), t1);
        QVERIFY(t1->hasDirectParent(list));
        Text *t2 = Text::fork(world, "val2");
        world->addDirectChild("t2", t2);
        list->set(0, t2);
        QCOMPARE(list->size(), 1);
        QCOMPARE(list->get(0), t2);
        QVERIFY(!t1->hasDirectParent(list));
        QVERIFY(t2->hasDirectParent(list));
        Text *t3 = Text::fork(world, "val3");
        world->addDirectChild("t3", t3);
        list->insert(0, t3);
        QCOMPARE(list->size(), 2);
        QCOMPARE(list->get(0), t3);
        QCOMPARE(list->get(1), t2);

        List *list2 = list->fork();
        QCOMPARE(list2->size(), 2);
        QCOMPARE(list2->get(0), t3);
        QCOMPARE(list2->get(1), t2);
    }

    void ListTest::remove() { // TODO: test with fork
        World w; World *world = &w;
        List *list = List::fork(world);
        Text *t1 = Text::fork(world, "val1");
        Text *t2 = Text::fork(world, "val2");
        Text *t3 = Text::fork(world, "val3");
        list->append(t1);
        list->append(t2);
        list->append(t3);
        QCOMPARE(list->size(), 3);
        QCOMPARE(list->get(0), t1);
        QCOMPARE(list->get(1), t2);
        QCOMPARE(list->get(2), t3);
        QCATCH(list->remove(-1), IndexOutOfBoundsException);
        QCATCH(list->remove(3), IndexOutOfBoundsException);
        list->remove(0);
        QCOMPARE(list->size(), 2);
        QCOMPARE(list->get(0), t2);
        QCOMPARE(list->get(1), t3);
        list->remove(1);
        QCOMPARE(list->size(), 1);
        QCOMPARE(list->get(0), t2);
        list->remove(0);
        QCOMPARE(list->size(), 0);
    }

    void ListTest::clear() {
        World w; World *world = &w;
        List *list = List::fork(world);
        Text *t1 = Text::fork(world, "val1");
        Text *t2 = Text::fork(world, "val2");
        Text *t3 = Text::fork(world, "val3");
        list->append(t1);
        list->append(t2);
        list->append(t3);
        QCOMPARE(list->size(), 3);
        list->clear();
        QCOMPARE(list->size(), 0);
    }
}
