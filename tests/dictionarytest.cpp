#include "tests/dictionarytest.h"

namespace Child {
    void DictionaryTest::initialize() {
        QVERIFY(Node::root()->child("Object")->hasDirectChild("Dictionary"));
    }

    void DictionaryTest::getAndSet() { // TODO: more tests with fork and/or removed key
        World w; World *world = &w;
        Dictionary *dict = CHILD_DICTIONARY(world->child("Dictionary"))->fork();
        QCOMPARE(dict->size(), 0);
        QVERIFY(dict->isEmpty());
        QCATCH(dict->get(""), ArgumentException);
        QCATCH(dict->get("missing"), NotFoundException);
        QCATCH(dict->set("a", NULL), NullPointerException);
        Text *t1 = CHILD_TEXT(world->child("Text"))->fork("val1");
        world->addDirectChild("t1", t1);
        dict->set("a", t1);
        QCOMPARE(dict->size(), 1);
        QVERIFY(!dict->isEmpty());
        QCOMPARE(dict->get("a"), t1);
        QVERIFY(t1->hasDirectParent(dict));
        Text *t2 = CHILD_TEXT(world->child("Text"))->fork("val2");
        world->addDirectChild("t2", t2);
        dict->set("a", t2);
        QCOMPARE(dict->size(), 1);
        QCOMPARE(dict->get("a"), t2);
        QVERIFY(!t1->hasDirectParent(dict));
        QVERIFY(t2->hasDirectParent(dict));
        Text *t3 = CHILD_TEXT(world->child("Text"))->fork("val3");
        world->addDirectChild("t3", t3);
        dict->set("b", t3);
        QCOMPARE(dict->size(), 2);
        QCOMPARE(dict->get("b"), t3);
        Text *t4 = CHILD_TEXT(world->child("Text"))->fork("val4");
        world->addDirectChild("t4", t4);
        dict->set("", t4); // anonymous key
        QCOMPARE(dict->size(), 3);
        Text *t5 = CHILD_TEXT(world->child("Text"))->fork("val5");
        world->addDirectChild("t5", t5);
        dict->set("", t5); // anonymous key
        QCOMPARE(dict->size(), 4);

        Dictionary *dict2 = dict->fork();
        QCOMPARE(dict2->size(), 4);
        QCOMPARE(dict->get("a"), t2);
        QCOMPARE(dict->get("b"), t3);
    }

    void DictionaryTest::keys() { // TODO: test with fork and/or removed key
        World w; World *world = &w;
        Dictionary *dict = CHILD_DICTIONARY(world->child("Dictionary"))->fork();
        QList<QString> keys = dict->keys();
        QVERIFY(keys.isEmpty());
        Text *t1 = CHILD_TEXT(world->child("Text"))->fork("val1");
        Text *t2 = CHILD_TEXT(world->child("Text"))->fork("val2");
        Text *t3 = CHILD_TEXT(world->child("Text"))->fork("val3");
        dict->set("t1", t1);
        dict->set("t2", t2);
        dict->set("t3", t3);
        keys = dict->keys();
        QCOMPARE(keys.size(), 3);
        QCOMPARE(QStringList(keys).join(", "), QString("t1, t2, t3"));
    }

    void DictionaryTest::remove() { // TODO: test with fork
        World w; World *world = &w;
        Dictionary *dict = CHILD_DICTIONARY(world->child("Dictionary"))->fork();
        Text *t1 = CHILD_TEXT(world->child("Text"))->fork("val1");
        Text *t2 = CHILD_TEXT(world->child("Text"))->fork("val2");
        Text *t3 = CHILD_TEXT(world->child("Text"))->fork("val3");
        dict->set("t1", t1);
        dict->set("t2", t2);
        dict->set("t3", t3);
        QCOMPARE(dict->size(), 3);
        QVERIFY(dict->hasKey("t1"));
        QCOMPARE(dict->get("t1"), t1);
        QCOMPARE(QStringList(dict->keys()).join(", "), QString("t1, t2, t3"));
        QCATCH(dict->remove(""), ArgumentException);
        QCATCH(dict->remove("missing"), NotFoundException);
        dict->remove("t1");
        QCOMPARE(dict->size(), 2);
        QVERIFY(!dict->hasKey("t1"));
        QCATCH(dict->get("t1"), NotFoundException);
        QCOMPARE(QStringList(dict->keys()).join(", "), QString("t2, t3"));
        QCATCH(dict->remove("t1"), RuntimeException);
        dict->remove("t2");
        dict->remove("t3");
        QCOMPARE(dict->size(), 0);
        QCOMPARE(QStringList(dict->keys()).join(", "), QString(""));
    }

    void DictionaryTest::clear() {
        World w; World *world = &w;
        Dictionary *dict = CHILD_DICTIONARY(world->child("Dictionary"))->fork();
        Text *t1 = CHILD_TEXT(world->child("Text"))->fork("val1");
        Text *t2 = CHILD_TEXT(world->child("Text"))->fork("val2");
        Text *t3 = CHILD_TEXT(world->child("Text"))->fork("val3");
        dict->set("t1", t1);
        dict->set("t2", t2);
        dict->set("t3", t3);
        dict->clear();
        QCOMPARE(dict->size(), 0);
        QCOMPARE(QStringList(dict->keys()).join(", "), QString(""));
    }
}
