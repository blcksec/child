#include "tests/nodetest.h"

namespace Child {
    void NodeTest::initialize() {
        QVERIFY(Node::root()->origin().isNull());
        QVERIFY(Node::root()->hasDirectParent(Node::root()));
    }

    void NodeTest::setOrigin() {
        NodePtr obj1(new Node());
        NodePtr obj2(new Node());
        QCOMPARE(obj2->origin(), Node::find("Node"));
        QCATCH(obj2->setOrigin(NodePtr()), NullPointerException);
        obj2->setOrigin(obj1);
        QCOMPARE(obj2->origin(), obj1);
    }

    void NodeTest::fork() {
        NodePtr obj1(new Node());
        NodePtr obj2(obj1->fork());
        QCOMPARE(obj2->origin(), obj1);
    }

    void NodeTest::addExtension() {
        NodePtr node1(new Node());
        NodePtr node2(new Node());
        NodePtr obj(new Node());
        QCOMPARE(obj->extensions().size(), 0);
        QCATCH(obj->addExtension(NodePtr()), NullPointerException);
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
        NodePtr node1(new Node());
        NodePtr node2(new Node());
        NodePtr obj(new Node());
        QCOMPARE(obj->extensions().size(), 0);
        obj->addExtension(node1);
        QCOMPARE(obj->extensions().size(), 1);
        QCATCH(obj->removeExtension(NodePtr()), NullPointerException);
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
        NodePtr node1(new Node());
        NodePtr node2(new Node());
        NodePtr obj(new Node());
        obj->addExtension(node1);
        QCATCH(obj->hasExtension(NodePtr()), NullPointerException);
        QVERIFY(obj->hasExtension(node1));
        QVERIFY(!obj->hasExtension(node2));
        obj->addExtension(node2);
        QVERIFY(obj->hasExtension(node2));
    }

    void NodeTest::addChild() {
        //        Node w; Node *world = &w;
        //        Node *window1 = new Node;
        //        Node *window2 = new Node;
        //        QCATCH(world->addDirectChild("window", NULL), NullPointerException);
        //        QCATCH(world->addDirectChild("", window1), ArgumentException);
        //        world->addDirectChild("window", window1);
        //        QCOMPARE(world->directChild("window"), window1);
        //        QCATCH(world->addDirectChild("window", window1), DuplicateException);
        //        world->addDirectChild("window2", window2);
        //        QCOMPARE(world->children().size(), 2);
        //        QCOMPARE(world->directChild("window2"), window2);
    }

    void NodeTest::setChild() {
        //        Node w; Node *world = &w;
        //        Node *window1 = new Node;
        //        Node *window2 = new Node;
        //        QCATCH(world->setDirectChild("window", NULL), NullPointerException);
        //        QCATCH(world->setDirectChild("", window1), ArgumentException);
        //        QCATCH(world->setDirectChild("window", window1), NotFoundException);
        //        world->addDirectChild("window", window1);
        //        QCOMPARE(world->directChild("window"), window1);
        //        world->setDirectChild("window", window1);
        //        QCOMPARE(world->children().size(), 1);
        //        QCOMPARE(world->directChild("window"), window1);
        //        world->setDirectChild("window", window2);
        //        QCOMPARE(world->children().size(), 1);
        //        QCOMPARE(world->directChild("window"), window2);
    }

    void NodeTest::removeChild() {
//        Node w; Node *world = &w;
//        Node *window1 = new Node;
//        window1->addParent("window1", world);
//        Node *button1 = new Node;
//        QCATCH(window1->removeDirectChild(""), ArgumentException);
//        QCATCH(window1->removeDirectChild("button"), NotFoundException);
//        button1->addParent("button", window1);
//        QCOMPARE(window1->children().size(), 1);
//        window1->removeDirectChild("button");
//        QCOMPARE(window1->children().size(), 0);
    }

    void NodeTest::hasDirectChild() {
        NodePtr world(new Node());
        NodePtr country(new Node());
        QVERIFY(!world->hasDirectChild("France"));
        QVERIFY(world->hasDirectChild(country).isNull());
        world->setChild("France", country);
        QVERIFY(world->hasDirectChild("France"));
        QCOMPARE(world->hasDirectChild(country), QString("France"));
    }

    void NodeTest::hasChild() {
    }

    void NodeTest::inspect() {
//        Node w; Node *world = &w;
//        Node *window = new Node;
//        world->addDirectChild("window", window);
//        Node *person = new Node;
//        window->addDirectChild("person1", person);
//        window->addDirectChild("person2", person);
//        Node *first = new Node;
//        Node *last = new Node;
//        person->addDirectChild("first_name", first);
//        person->addDirectChild("last_name", last);
//        QCOMPARE(person->inspect(), person->hexMemoryAddress() + ": [person1, person2] => [last_name, first_name]");
    }

//    void NodeTest::child() {
//        Node w; Node *world = &w;
//        world->addDirectChild("World", world);
//        Node *Window = new Node;
//        world->addDirectChild("Window", Window);
//        Node *close = new Node;
//        Window->addDirectChild("close", close);
//        Node *window1 = Window->fork();
//        Node *window2 = Window->fork();
//        world->addDirectChild("win1", window1);
//        world->addDirectChild("win2", window2);
//        Node *button = new Node;
//        window1->addDirectChild("button", button);
//        QCATCH(button->child(""), ArgumentException);
//        QCATCH(button->child("win3"), NotFoundException);
//        QCOMPARE(world->child("Window"), Window);
//        QCOMPARE(Window->child("Window"), Window);
//        QCOMPARE(world->child("win1"), window1);
//        QCATCH(world->child("button"), NotFoundException);
//        QCOMPARE(window1->child("win2"), window2);
//        QCOMPARE(button->child("World"), world);
//        QCOMPARE(button->child("win2"), window2);
//        Node *virtualClose = window1->child("close");
//        QVERIFY(virtualClose->isVirtual());
//        QVERIFY(virtualClose->directOriginIs(close));
//        QCOMPARE(button->child("close"), virtualClose);
//        Node *virtualButton = button->fork()->setIsVirtual(true);
//        QCOMPARE(window1->forks().size(), 0);
//        Node *virtualClose2 = virtualButton->child("close");
//        QVERIFY(virtualClose2->directOriginIs(virtualClose));
//        QCOMPARE(window1->forks().size(), 1);
//        QVERIFY(virtualClose2->hasDirectParent(window1->forks().first()));
//    }

//    void NodeTest::setChild() {
//        Node w; Node *world = &w;
//        Node *Person = world->addDirectChild("Person", new Node);
//        Node *mvila = world->addDirectChild("mvila", Person->fork());
//        Node *manu = new Node;
//        Node *vila = new Node;
//        QCATCH(mvila->setChild("", manu), ArgumentException);
//        QCATCH(mvila->setChild("first_name", NULL), NullPointerException);
//        QCATCH(mvila->setChild("first_name", manu), NotFoundException);
//        Person->addDirectChild("first_name", new Node);
//        Person->addDirectChild("last_name", new Node);
//        QCOMPARE(mvila->setChild("first_name", manu), manu);
//        QCOMPARE(mvila->setChild("last_name", vila), vila);
//        QCOMPARE(mvila->directChild("first_name"), manu);
//        QCOMPARE(mvila->directChild("last_name"), vila);
//    }
}
