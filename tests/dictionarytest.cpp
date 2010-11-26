#include "tests/dictionarytest.h"

namespace Child {
    void DictionaryTest::initialize() {
        QVERIFY(Node::root()->child("Object")->hasDirectChild("Dictionary"));
    }

    void DictionaryTest::getAndSet() { // TODO: more tests with fork and/or removed key
        World w; World *world = &w;
        NewDictionary *dict = NewDictionary::fork(world);
        QCOMPARE(dict->size(), 0);
        QVERIFY(dict->isEmpty());
        QCATCH(dict->get(NULL), NullPointerException);
        QCATCH(dict->get(Text::fork(world, "missing")), NotFoundException);
        QCATCH(dict->set(Text::fork(world, "a"), NULL), NullPointerException);
        Text *t1 = Text::fork(world, "val1");
        Text *k1 = Text::fork(world, "k1");
        dict->set(k1, t1);
        QCOMPARE(dict->size(), 1);
        QVERIFY(dict->isNotEmpty());
        QCOMPARE(dict->get(k1), t1);
        QCOMPARE(dict->get(Text::fork(world, "k1")), t1);
        QVERIFY(t1->hasDirectParent(dict));
        Text *t2 = Text::fork(world, "val2");
        dict->set(k1, t2);
        QCOMPARE(dict->size(), 1);
        QCOMPARE(dict->get(k1), t2);
        QVERIFY(!t1->hasDirectParent(dict));
        QVERIFY(t2->hasDirectParent(dict));
        dict->set(Text::fork(world, "k1"), t2);
        QCOMPARE(dict->size(), 1);
        QCOMPARE(dict->get(k1), t2);
        QVERIFY(!t1->hasDirectParent(dict));
        QVERIFY(t2->hasDirectParent(dict));
        Text *t3 = Text::fork(world, "val3");
        Text *k2 = Text::fork(world, "k2");
        dict->set(k2, t3);
        QCOMPARE(dict->size(), 2);
        QCOMPARE(dict->get(k2), t3);

        NewDictionary *dict2 = dict->fork();
        QCOMPARE(dict2->size(), 2);
        QCOMPARE(dict->get(k1), t2);
        QCOMPARE(dict->get(k2), t3);
    }

    void DictionaryTest::keys() { // TODO: test with fork and/or removed key
        World w; World *world = &w;
        NewDictionary *dict = NewDictionary::fork(world);
        NodeList keys = dict->keys();
        QVERIFY(keys.isEmpty());
        Text *t1 = Text::fork(world, "val1");
        Text *t2 = Text::fork(world, "val2");
        Text *t3 = Text::fork(world, "val3");
        dict->set(Text::fork(world, "t1"), t1);
        dict->set(Text::fork(world, "t2"), t2);
        dict->set(Text::fork(world, "t3"), t3);
        keys = dict->keys();
        QCOMPARE(keys.size(), 3);
        List *list = List::fork(world, keys);
        QVERIFY(list->hasValue(Text::fork(world, "t1")));
        QVERIFY(list->hasValue(Text::fork(world, "t2")));
        QVERIFY(list->hasValue(Text::fork(world, "t3")));
        QVERIFY(!list->hasValue(Text::fork(world, "t4")));
    }

    void DictionaryTest::remove() { // TODO: test with fork
        World w; World *world = &w;
        NewDictionary *dict = NewDictionary::fork(world);
        Text *t1 = Text::fork(world, "val1");
        Text *t2 = Text::fork(world, "val2");
        Text *t3 = Text::fork(world, "val3");
        dict->set(Text::fork(world, "t1"), t1);
        dict->set(Text::fork(world, "t2"), t2);
        dict->set(Text::fork(world, "t3"), t3);
        QCOMPARE(dict->size(), 3);
        QVERIFY(dict->hasKey(Text::fork(world, "t1")));
        QCOMPARE(dict->get(Text::fork(world, "t1")), t1);
        QCATCH(dict->remove(NULL), NullPointerException);
        QCATCH(dict->remove(Text::fork(world, "missing")), NotFoundException);
        dict->remove(Text::fork(world, "t1"));
        QCOMPARE(dict->size(), 2);
        QVERIFY(!dict->hasKey(Text::fork(world, "t1")));
        QCATCH(dict->get(Text::fork(world, "t1")), NotFoundException);
        QCATCH(dict->remove(Text::fork(world, "t1")), NotFoundException);
        dict->remove(Text::fork(world, "t2"));
        QCOMPARE(dict->size(), 1);
        dict->remove(Text::fork(world, "t3"));
        QCOMPARE(dict->size(), 0);
    }

    void DictionaryTest::clear() {
        World w; World *world = &w;
        NewDictionary *dict = NewDictionary::fork(world);
        Text *t1 = Text::fork(world, "val1");
        Text *t2 = Text::fork(world, "val2");
        Text *t3 = Text::fork(world, "val3");
        dict->set(Text::fork(world, "t1"), t1);
        dict->set(Text::fork(world, "t2"), t2);
        dict->set(Text::fork(world, "t3"), t3);
        dict->clear();
        QCOMPARE(dict->size(), 0);
    }
}
