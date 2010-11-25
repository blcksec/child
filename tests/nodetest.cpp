#include "tests/nodetest.h"

namespace Child {
    void NodeTest::initialize() {
        QVERIFY(Node::root()->hasDirectParent(Node::root()));
    }

    void NodeTest::setOrigin() {
        Node *obj1 = new Node;
        Node *obj2 = new Node;
        QVERIFY(obj2->origin() == NULL);
        QCOMPARE(obj1->forks().size(), 0);
        QCATCH(obj2->setOrigin(NULL), NullPointerException);
        obj2->setOrigin(obj1);
        QCOMPARE(obj2->origin(), obj1);
        QCOMPARE(obj1->forks().size(), 1);
        Node *obj3 = new Node;
        obj3->setOrigin(obj1);
        QCOMPARE(obj1->forks().size(), 2);
        delete obj3;
        QCOMPARE(obj1->forks().size(), 1);
        Node *obj4 = new Node;
        obj2->setOrigin(obj4);
        QCOMPARE(obj2->origin(), obj4);
        QCOMPARE(obj4->forks().size(), 1);
        QCOMPARE(obj1->forks().size(), 0);
        delete obj2;
        delete obj4;
        delete obj1;
    }

    void NodeTest::unsetOrigin() {
        Node *obj1 = new Node;
        Node *obj2 = new Node;
        obj2->setOrigin(obj1);
        QCOMPARE(obj2->origin(), obj1);
        QCOMPARE(obj1->forks().size(), 1);
        obj2->unsetOrigin();
        QVERIFY(obj2->origin() == NULL);
        QCOMPARE(obj1->forks().size(), 0);
        delete obj2;
        delete obj1;
    }

    void NodeTest::directOriginIs() {
        Node *obj1 = new Node;
        Node *obj2 = new Node;
        obj2->setOrigin(obj1);
        QCATCH(obj2->directOriginIs(NULL), NullPointerException);
        QVERIFY(obj2->directOriginIs(obj1));
        delete obj2;
        delete obj1;
    }

    void NodeTest::fork() {
        Node *obj1 = new Node;
        QCOMPARE(obj1->forks().size(), 0);
        Node *obj2 = obj1->fork();
        QCOMPARE(obj1->forks().size(), 1);
        QCOMPARE(obj1->forks()[0], obj2);
        QCOMPARE(obj2->origin(), obj1);
        Node *obj3 = obj1->fork();
        QCOMPARE(obj1->forks().size(), 2);
        QCOMPARE(obj1->forks()[1], obj3);
        QCOMPARE(obj3->origin(), obj1);
        delete obj3;
        delete obj2;
        delete obj1;
    }

    void NodeTest::addExtension() {
        Node *node1 = new Node;
        Node *node2 = new Node;
        Node *obj = new Node;
        QCOMPARE(node1->extendedNodes().size(), 0);
        QCOMPARE(node2->extendedNodes().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        QCATCH(obj->addExtension(NULL), NullPointerException);
        obj->addExtension(node1);
        QCOMPARE(node1->extendedNodes().size(), 1);
        QCOMPARE(node1->extendedNodes()[0], obj);
        QCOMPARE(obj->extensions().size(), 1);
        QCOMPARE(obj->extensions()[0], node1);
        QCATCH(obj->addExtension(node1), DuplicateException);
        obj->addExtension(node2);
        QCOMPARE(node2->extendedNodes().size(), 1);
        QCOMPARE(node2->extendedNodes()[0], obj);
        QCOMPARE(obj->extensions().size(), 2);
        QCOMPARE(obj->extensions()[1], node2);
        QCATCH(obj->addExtension(node2), DuplicateException);
        delete obj;
        delete node1;
        delete node2;
    }

    void NodeTest::prependExtension() {
        Node *node1 = new Node;
        Node *node2 = new Node;
        Node *obj = new Node;
        QCOMPARE(node1->extendedNodes().size(), 0);
        QCOMPARE(node2->extendedNodes().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        QCATCH(obj->prependExtension(NULL), NullPointerException);
        obj->prependExtension(node1);
        QCOMPARE(node1->extendedNodes().size(), 1);
        QCOMPARE(node1->extendedNodes()[0], obj);
        QCOMPARE(obj->extensions().size(), 1);
        QCOMPARE(obj->extensions()[0], node1);
        QCATCH(obj->prependExtension(node1), DuplicateException);
        obj->prependExtension(node2);
        QCOMPARE(node2->extendedNodes().size(), 1);
        QCOMPARE(node2->extendedNodes()[0], obj);
        QCOMPARE(obj->extensions().size(), 2);
        QCOMPARE(obj->extensions()[0], node2);
        QCOMPARE(obj->extensions()[1], node1);
        QCATCH(obj->prependExtension(node2), DuplicateException);
        delete obj;
        delete node1;
        delete node2;
    }

    void NodeTest::removeExtension() {
        Node *node1 = new Node;
        Node *node2 = new Node;
        Node *obj = new Node;
        QCOMPARE(node1->extendedNodes().size(), 0);
        QCOMPARE(node2->extendedNodes().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        obj->addExtension(node1);
        QCOMPARE(node1->extendedNodes().size(), 1);
        QCOMPARE(obj->extensions().size(), 1);
        QCATCH(obj->removeExtension(NULL), NullPointerException);
        QCATCH(obj->removeExtension(node2), NotFoundException);
        obj->removeExtension(node1);
        QCOMPARE(node1->extendedNodes().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        obj->addExtension(node1);
        obj->addExtension(node2);
        QCOMPARE(obj->extensions().size(), 2);
        obj->removeExtension(node1);
        QCOMPARE(obj->extensions().size(), 1);
        obj->removeExtension(node2);
        QCOMPARE(obj->extensions().size(), 0);
        delete obj;
        delete node1;
        delete node2;
    }

    void NodeTest::hasExtension() {
        Node *node1 = new Node;
        Node *node2 = new Node;
        Node *obj = new Node;
        obj->addExtension(node1);
        QCATCH(obj->hasExtension(NULL), NullPointerException);
        QVERIFY(obj->hasExtension(node1));
        QVERIFY(!obj->hasExtension(node2));
        obj->addExtension(node2);
        QVERIFY(obj->hasExtension(node2));
        delete obj;
        delete node1;
        delete node2;
    }

    void NodeTest::addParent() {
        Node *window1 = new Node;
        Node *window2 = new Node;
        Node *button1 = new Node;
        Node *button2 = new Node;
        QCOMPARE(window1->children().size(), 0);
        QCOMPARE(button1->parents().size(), 0);
        QCATCH(button1->addParent("button", NULL), NullPointerException);
        QCATCH(button1->addParent("", window1), ArgumentException);
        button1->addParent("button", window1);
        QCOMPARE(window1->children().size(), 1);
        QCOMPARE(window1->children().value("button"), button1);
        QCOMPARE(button1->parents().size(), 1);
        QCOMPARE(button1->parents().value("button"), window1);
        QCATCH(button1->addParent("button", window1), DuplicateException);
        QCATCH(button2->addParent("button", window1), DuplicateException);
        button1->addParent("button", window2);
        QCOMPARE(window2->children().size(), 1);
        QCOMPARE(window2->children().value("button"), button1);
        QCOMPARE(button1->parents().size(), 2);
        NodeList windows = button1->parents().values("button");
        QVERIFY(windows.contains(window1));
        QVERIFY(windows.contains(window2));
        delete(button1);
        delete(button2);
        delete(window1);
        delete(window2);
    }

    void NodeTest::removeParent() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        window1->addParent("window1", world);
        Node *window2 = new Node;
        window2->addParent("window2", world);
        Node *button1 = new Node;
        button1->addParent("keepalive", world);
        QCATCH(button1->removeParent("button", NULL), NullPointerException);
        QCATCH(button1->removeParent("", window1), ArgumentException);
        QCATCH(button1->removeParent("button", window1), NotFoundException);
        button1->addParent("button", window1);
        button1->removeParent("button", window1);
        QCOMPARE(window1->children().size(), 0);
        QCOMPARE(button1->parents().size(), 1);
        button1->addParent("button", window1);
        button1->addParent("button", window2);
        QCOMPARE(button1->parents().size(), 3);
        button1->removeParent("button", window1);
        QCOMPARE(button1->parents().size(), 2);
        button1->removeParent("button", window2);
        QCOMPARE(button1->parents().size(), 1);
    }

    void NodeTest::directChild() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        QCATCH(world->directChild(""), ArgumentException);
        QCATCH(world->directChild("window"), NotFoundException);
        world->addDirectChild("window", window1);
        QCOMPARE(world->directChild("window"), window1);
    }

    void NodeTest::hasDirectChild() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        QCATCH(world->hasDirectChild(""), ArgumentException);
        QVERIFY(!world->hasDirectChild("window"));
        world->addDirectChild("window", window1);
        QVERIFY(world->hasDirectChild("window"));
    }

    void NodeTest::addDirectChild() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        Node *window2 = new Node;
        QCATCH(world->addDirectChild("window", NULL), NullPointerException);
        QCATCH(world->addDirectChild("", window1), ArgumentException);
        world->addDirectChild("window", window1);
        QCOMPARE(world->directChild("window"), window1);
        QCATCH(world->addDirectChild("window", window1), DuplicateException);
        world->addDirectChild("window2", window2);
        QCOMPARE(world->children().size(), 2);
        QCOMPARE(world->directChild("window2"), window2);
    }

    void NodeTest::setDirectChild() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        Node *window2 = new Node;
        QCATCH(world->setDirectChild("window", NULL), NullPointerException);
        QCATCH(world->setDirectChild("", window1), ArgumentException);
        QCATCH(world->setDirectChild("window", window1), NotFoundException);
        world->addDirectChild("window", window1);
        QCOMPARE(world->directChild("window"), window1);
        world->setDirectChild("window", window1);
        QCOMPARE(world->children().size(), 1);
        QCOMPARE(world->directChild("window"), window1);
        world->setDirectChild("window", window2);
        QCOMPARE(world->children().size(), 1);
        QCOMPARE(world->directChild("window"), window2);
    }

    void NodeTest::removeDirectChild() {
        Node w; Node *world = &w;
        Node *window1 = new Node;
        window1->addParent("window1", world);
        Node *button1 = new Node;
        QCATCH(window1->removeDirectChild(""), ArgumentException);
        QCATCH(window1->removeDirectChild("button"), NotFoundException);
        button1->addParent("button", window1);
        QCOMPARE(window1->children().size(), 1);
        window1->removeDirectChild("button");
        QCOMPARE(window1->children().size(), 0);
    }

    void NodeTest::child() {
        Node w; Node *world = &w;
        world->addDirectChild("World", world);
        Node *Window = new Node;
        world->addDirectChild("Window", Window);
        Node *close = new Node;
        Window->addDirectChild("close", close);
        Node *window1 = Window->fork();
        Node *window2 = Window->fork();
        world->addDirectChild("win1", window1);
        world->addDirectChild("win2", window2);
        Node *button = new Node;
        window1->addDirectChild("button", button);
        QCATCH(button->child(""), ArgumentException);
        QCATCH(button->child("win3"), NotFoundException);
        QCOMPARE(world->child("Window"), Window);
        QCOMPARE(Window->child("Window"), Window);
        QCOMPARE(world->child("win1"), window1);
        QCATCH(world->child("button"), NotFoundException);
        QCOMPARE(window1->child("win2"), window2);
        QCOMPARE(button->child("World"), world);
        QCOMPARE(button->child("win2"), window2);
        Node *virtualClose = window1->child("close");
        QVERIFY(virtualClose->isVirtual());
        QVERIFY(virtualClose->directOriginIs(close));
        QCOMPARE(button->child("close"), virtualClose);
        Node *virtualButton = button->fork()->setIsVirtual(true);
        QCOMPARE(window1->forks().size(), 0);
        Node *virtualClose2 = virtualButton->child("close");
        QVERIFY(virtualClose2->directOriginIs(virtualClose));
        QCOMPARE(window1->forks().size(), 1);
        QVERIFY(virtualClose2->hasDirectParent(window1->forks().first()));
    }

    void NodeTest::setChild() {
        Node w; Node *world = &w;
        Node *Person = world->addDirectChild("Person", new Node);
        Node *mvila = world->addDirectChild("mvila", Person->fork());
        Node *manu = new Node;
        Node *vila = new Node;
        QCATCH(mvila->setChild("", manu), ArgumentException);
        QCATCH(mvila->setChild("first_name", NULL), NullPointerException);
        QCATCH(mvila->setChild("first_name", manu), NotFoundException);
        Person->addDirectChild("first_name", new Node);
        Person->addDirectChild("last_name", new Node);
        QCOMPARE(mvila->setChild("first_name", manu), manu);
        QCOMPARE(mvila->setChild("last_name", vila), vila);
        QCOMPARE(mvila->directChild("first_name"), manu);
        QCOMPARE(mvila->directChild("last_name"), vila);
    }

    void NodeTest::inspect() {
        Node w; Node *world = &w;
        Node *window = new Node;
        world->addDirectChild("window", window);
        Node *person = new Node;
        window->addDirectChild("person1", person);
        window->addDirectChild("person2", person);
        Node *first = new Node;
        Node *last = new Node;
        person->addDirectChild("first_name", first);
        person->addDirectChild("last_name", last);
        QCOMPARE(person->inspect(), person->hexMemoryAddress() + ": [person1, person2] => [last_name, first_name]");
    }
}
