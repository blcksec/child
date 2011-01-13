#include "tests/nodetest.h"

CHILD_BEGIN

void NodeTest::initialize() {
    QVERIFY(Node::root()->origin() == Node::root());
    QVERIFY(Node::root()->hasDirectParent(Node::root()));
}

void NodeTest::setOrigin() {
    Node *obj1 = CHILD_NODE();
    Node *obj2 = CHILD_NODE();
    QCOMPARE(obj2->origin(), context()->child("Node"));
    QCATCH(obj2->setOrigin(NULL), NullPointerException);
    obj2->setOrigin(obj1);
    QCOMPARE(obj2->origin(), obj1);
}

void NodeTest::fork() {
    Node *obj1 = CHILD_NODE();
    Node *obj2 = obj1->fork();
    QCOMPARE(obj2->origin(), obj1);
}

void NodeTest::addExtension() {
    Node *node1 = CHILD_NODE();
    Node *node2 = CHILD_NODE();
    Node *obj = CHILD_NODE();
    QCOMPARE(obj->extensions().size(), 0);
    QCATCH(obj->addExtension(NULL), NullPointerException);
    obj->addExtension(node1);
    QCOMPARE(obj->extensions().size(), 1);
    QCOMPARE(obj->extensions()[0], node1);
    QCATCH(obj->addExtension(node1), DuplicateException);
    obj->addExtension(node2);
    QCOMPARE(obj->extensions().size(), 2);
    QCOMPARE(obj->extensions()[1], node2);
    QCATCH(obj->addExtension(node2), DuplicateException);
}

void NodeTest::removeExtension() {
    Node *node1 = CHILD_NODE();
    Node *node2 = CHILD_NODE();
    Node *obj = CHILD_NODE();
    QCOMPARE(obj->extensions().size(), 0);
    obj->addExtension(node1);
    QCOMPARE(obj->extensions().size(), 1);
    QCATCH(obj->removeExtension(NULL), NullPointerException);
    QCATCH(obj->removeExtension(node2), NotFoundException);
    obj->removeExtension(node1);
    QCOMPARE(obj->extensions().size(), 0);
    obj->addExtension(node1);
    obj->addExtension(node2);
    QCOMPARE(obj->extensions().size(), 2);
    obj->removeExtension(node1);
    QCOMPARE(obj->extensions().size(), 1);
    obj->removeExtension(node2);
    QCOMPARE(obj->extensions().size(), 0);
}

void NodeTest::hasExtension() {
    Node *node1 = CHILD_NODE();
    Node *node2 = CHILD_NODE();
    Node *obj = CHILD_NODE();
    obj->addExtension(node1);
    QCATCH(obj->hasExtension(NULL), NullPointerException);
    QVERIFY(obj->hasExtension(node1));
    QVERIFY(!obj->hasExtension(node2));
    obj->addExtension(node2);
    QVERIFY(obj->hasExtension(node2));
}

void NodeTest::addChild() {
    Node *world = CHILD_NODE();
    Node *country1 = CHILD_NODE();
    Node *country2 = CHILD_NODE();
    QCATCH(world->addChild("France", NULL), NullPointerException);
    world->addChild("France", country1);
    QCOMPARE(world->child("France"), country1);
    QCATCH(world->addChild("France", country1), DuplicateException);
    world->addChild("Spain", country2);
    QCOMPARE(world->child("Spain"), country2);
    QCOMPARE(world->children().size(), 2);
    Node *newWorld = world->fork();
    QCATCH(newWorld->addChild("France", country1), DuplicateException);
    Node *city = CHILD_NODE();
    country1->addChild("France", city);
    QCOMPARE(country1->child("France"), city);
}

void NodeTest::setChild() {
    Node *world = CHILD_NODE();
    Node *country1 = CHILD_NODE();
    Node *country2 = CHILD_NODE();
    QCATCH(world->setChild("France", NULL), NullPointerException);
    QCATCH(world->setChild("France", country1), NotFoundException);
    world->addChild("France", country1);
    QCOMPARE(world->child("France"), country1);
    world->setChild("France", country1);
    QCOMPARE(world->child("France"), country1);
    world->setChild("France", country2);
    QCOMPARE(world->child("France"), country2);
    QCOMPARE(world->children().size(), 1);
    QCOMPARE(country2->parents().size(), 1);
    QCOMPARE(country1->parents().size(), 0);
    Node *newWorld = world->fork();
    newWorld->setChild("France", country1);
    QCOMPARE(newWorld->child("France"), country1);
}

void NodeTest::removeChild() {
    Node *world = CHILD_NODE();
    Node *country = CHILD_NODE();
    QCATCH(world->removeChild("France"), NotFoundException);
    world->addChild("France", country);
    QVERIFY(world->hasChild("France"));
    QCOMPARE(world->child("France"), country);
    QCOMPARE(world->children().size(), 1);
    QCATCH(world->addChild("France", country), DuplicateException);
    world->removeChild("France");
    QVERIFY(!world->hasChild("France"));
    QCATCH(world->child("France"), NotFoundException);
    QCATCH(world->setChild("France", country), NotFoundException);
    QVERIFY(world->children().isEmpty());
    QCATCH(world->removeChild("France"), NotFoundException);
    world->addChild("France", country);
    QVERIFY(world->hasChild("France"));
    QCOMPARE(world->child("France"), country);
    QCOMPARE(world->children().size(), 1);
    QCATCH(world->addChild("France", country), DuplicateException);
    world->removeChild("France");
    Node *newWorld = world->fork();
    QVERIFY(!newWorld->hasChild("France"));
    QCATCH(newWorld->child("France"), NotFoundException);
    QCATCH(newWorld->setChild("France", country), NotFoundException);
    QVERIFY(newWorld->children().isEmpty());
    QCATCH(newWorld->removeChild("France"), NotFoundException);
    newWorld->addChild("France", country);
    QVERIFY(newWorld->hasChild("France"));
    QCOMPARE(newWorld->child("France"), country);
    QCOMPARE(newWorld->children().size(), 1);
    QCATCH(newWorld->addChild("France", country), DuplicateException);
}

void NodeTest::hasDirectChild() {
    Node *world = CHILD_NODE();
    Node *country = CHILD_NODE();
    QVERIFY(!world->hasDirectChild("France"));
    QVERIFY(world->hasDirectChild(country).isNull());
    world->addChild("France", country);
    QVERIFY(world->hasDirectChild("France"));
    QCOMPARE(world->hasDirectChild(country), QString("France"));
    world->removeChild("France");
    bool isRemoved;
    QVERIFY(!world->hasDirectChild("France", &isRemoved));
    QVERIFY(isRemoved);
}

void NodeTest::hasChild() {
    Node *world = CHILD_NODE();
    Node *country = CHILD_NODE();
    QVERIFY(!world->hasChild("France"));
    world->addChild("France", country);
    QVERIFY(world->hasChild("France"));
    Node *newNewWorld = world->fork()->fork();
    QVERIFY(newNewWorld->hasChild("France"));
}

CHILD_END
