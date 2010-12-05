#include "tests/dictionarytest.h"

CHILD_BEGIN

void DictionaryTest::initialize() {
    QCOMPARE(Dictionary::root()->origin(), Object::root());
    QCOMPARE(Object::root()->child("Dictionary"), Dictionary::root());
}

void DictionaryTest::getAndSet() { // TODO: more tests with fork and/or removed key
    DictionaryPtr dict = CHILD_DICTIONARY();
    QCOMPARE(dict->size(), 0);
    QVERIFY(dict->isEmpty());
    QCATCH(dict->get(NodePtr()), NullPointerException);
    QCATCH(dict->get(CHILD_TEXT("missing")), NotFoundException);
    QCATCH(dict->set(CHILD_TEXT("missing"), NodePtr()), NullPointerException);
    TextPtr t1 = CHILD_TEXT("val1");
    TextPtr k1 = CHILD_TEXT("key1");
    dict->set(k1, t1);
    QCOMPARE(dict->size(), 1);
    QVERIFY(dict->isNotEmpty());
    QCOMPARE(dict->get(k1), t1);
    QCOMPARE(dict->get(CHILD_TEXT("key1")), t1);
    QVERIFY(t1->hasDirectParent(dict));
    TextPtr t2 = CHILD_TEXT("val2");
    dict->set(k1, t2);
    QCOMPARE(dict->size(), 1);
    QCOMPARE(dict->get(k1), t2);
    QVERIFY(!t1->hasDirectParent(dict));
    QVERIFY(t2->hasDirectParent(dict));
    dict->set(CHILD_TEXT("key1"), t2);
    QCOMPARE(dict->size(), 1);
    QCOMPARE(dict->get(k1), t2);
    QVERIFY(!t1->hasDirectParent(dict));
    QVERIFY(t2->hasDirectParent(dict));
    TextPtr t3 = CHILD_TEXT("val3");
    TextPtr k2 = CHILD_TEXT("key2");
    dict->set(k2, t3);
    QCOMPARE(dict->size(), 2);
    QCOMPARE(dict->get(k2), t3);

    DictionaryPtr newDict(dict->fork());
    QCOMPARE(newDict->size(), 2);
    QCOMPARE(newDict->get(k1)->origin(), t2);
    QCOMPARE(newDict->get(k2)->origin(), t3);
}

void DictionaryTest::keys() { // TODO: test with fork and/or removed key
    DictionaryPtr dict = CHILD_DICTIONARY();
    NodeList keys = dict->keys();
    QVERIFY(keys.isEmpty());
    TextPtr t1 = CHILD_TEXT("val1");
    TextPtr k1 = CHILD_TEXT("key1");
    TextPtr t2 = CHILD_TEXT("val2");
    TextPtr k2 = CHILD_TEXT("key2");
    TextPtr t3 = CHILD_TEXT("val3");
    TextPtr k3 = CHILD_TEXT("key3");
    dict->set(k1, t1);
    dict->set(k2, t2);
    dict->set(k3, t3);
    keys = dict->keys();
    QCOMPARE(keys.size(), 3);
    ListPtr list = CHILD_LIST(keys);
    QVERIFY(list->hasValue(k1));
    QVERIFY(list->hasValue(CHILD_TEXT("key1")));
    QVERIFY(list->hasValue(k2));
    QVERIFY(list->hasValue(CHILD_TEXT("key2")));
    QVERIFY(list->hasValue(k3));
    QVERIFY(list->hasValue(CHILD_TEXT("key3")));
    QVERIFY(!list->hasValue(CHILD_TEXT("key4")));
}

void DictionaryTest::remove() { // TODO: test with fork
    DictionaryPtr dict = CHILD_DICTIONARY();
    TextPtr t1 = CHILD_TEXT("val1");
    TextPtr k1 = CHILD_TEXT("key1");
    TextPtr t2 = CHILD_TEXT("val2");
    TextPtr k2 = CHILD_TEXT("key2");
    TextPtr t3 = CHILD_TEXT("val3");
    TextPtr k3 = CHILD_TEXT("key3");
    dict->set(k1, t1);
    dict->set(k2, t2);
    dict->set(k3, t3);
    QCOMPARE(dict->size(), 3);
    QVERIFY(dict->hasKey(k1));
    QCOMPARE(dict->get(k1), t1);
    QVERIFY(t1->hasDirectParent(dict));
    QCATCH(dict->remove(NodePtr()), NullPointerException);
    QCATCH(dict->remove(CHILD_TEXT("missing")), NotFoundException);
    dict->remove(k1);
    QCOMPARE(dict->size(), 2);
    QVERIFY(!dict->hasKey(k1));
    QCATCH(dict->get(k1), NotFoundException);
    QVERIFY(!t1->hasDirectParent(dict));
    QCATCH(dict->remove(k1), NotFoundException);
    dict->remove(k2);
    QCOMPARE(dict->size(), 1);
    dict->remove(k3);
    QCOMPARE(dict->size(), 0);
}

void DictionaryTest::clear() {
    DictionaryPtr dict = CHILD_DICTIONARY();
    dict->set(CHILD_TEXT("key1"), CHILD_TEXT("val1"));
    dict->set(CHILD_TEXT("key2"), CHILD_TEXT("val2"));
    dict->set(CHILD_TEXT("key3"), CHILD_TEXT("val3"));
    dict->clear();
    QCOMPARE(dict->size(), 0);
}

CHILD_END
