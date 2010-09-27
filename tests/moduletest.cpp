#include "tests/moduletest.h"

namespace Child {
    void ModuleTest::initialize() {
        QVERIFY(Module::root()->hasDirectParent(Module::root()));
    }

    void ModuleTest::setBaseModule() {
        Module *obj1 = new Module;
        Module *obj2 = new Module;
        QVERIFY(obj2->baseModule() == NULL);
        QCOMPARE(obj1->forks().size(), 0);
        QCATCH(obj2->setBaseModule(NULL), NullPointerException);
        obj2->setBaseModule(obj1);
        QCOMPARE(obj2->baseModule(), obj1);
        QCOMPARE(obj1->forks().size(), 1);
        Module *obj3 = new Module;
        obj3->setBaseModule(obj1);
        QCOMPARE(obj1->forks().size(), 2);
        delete obj3;
        QCOMPARE(obj1->forks().size(), 1);
        Module *obj4 = new Module;
        obj2->setBaseModule(obj4);
        QCOMPARE(obj2->baseModule(), obj4);
        QCOMPARE(obj4->forks().size(), 1);
        QCOMPARE(obj1->forks().size(), 0);
        delete obj2;
        delete obj4;
        delete obj1;
    }

    void ModuleTest::unsetBaseModule() {
        Module *obj1 = new Module;
        Module *obj2 = new Module;
        obj2->setBaseModule(obj1);
        QCOMPARE(obj2->baseModule(), obj1);
        QCOMPARE(obj1->forks().size(), 1);
        obj2->unsetBaseModule();
        QVERIFY(obj2->baseModule() == NULL);
        QCOMPARE(obj1->forks().size(), 0);
        delete obj2;
        delete obj1;
    }

    void ModuleTest::isBasedOn() {
        Module *obj1 = new Module;
        Module *obj2 = new Module;
        obj2->setBaseModule(obj1);
        QCATCH(obj2->isBasedOn(NULL), NullPointerException);
        QVERIFY(obj2->isBasedOn(obj1));
        delete obj2;
        delete obj1;
    }

    void ModuleTest::fork() {
        Module *obj1 = new Module;
        QCOMPARE(obj1->forks().size(), 0);
        Module *obj2 = obj1->fork();
        QCOMPARE(obj1->forks().size(), 1);
        QCOMPARE(obj1->forks()[0], obj2);
        QCOMPARE(obj2->baseModule(), obj1);
        Module *obj3 = obj1->fork();
        QCOMPARE(obj1->forks().size(), 2);
        QCOMPARE(obj1->forks()[1], obj3);
        QCOMPARE(obj3->baseModule(), obj1);
        delete obj3;
        delete obj2;
        delete obj1;
    }

    void ModuleTest::addExtension() {
        Module *mod1 = new Module;
        Module *mod2 = new Module;
        Module *obj = new Module;
        QCOMPARE(mod1->extendedModules().size(), 0);
        QCOMPARE(mod2->extendedModules().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        QCATCH(obj->addExtension(NULL), NullPointerException);
        obj->addExtension(mod1);
        QCOMPARE(mod1->extendedModules().size(), 1);
        QCOMPARE(mod1->extendedModules()[0], obj);
        QCOMPARE(obj->extensions().size(), 1);
        QCOMPARE(obj->extensions()[0], mod1);
        QCATCH(obj->addExtension(mod1), DuplicateException);
        obj->addExtension(mod2);
        QCOMPARE(mod2->extendedModules().size(), 1);
        QCOMPARE(mod2->extendedModules()[0], obj);
        QCOMPARE(obj->extensions().size(), 2);
        QCOMPARE(obj->extensions()[1], mod2);
        QCATCH(obj->addExtension(mod2), DuplicateException);
        delete obj;
        delete mod1;
        delete mod2;
    }

    void ModuleTest::prependExtension() {
        Module *mod1 = new Module;
        Module *mod2 = new Module;
        Module *obj = new Module;
        QCOMPARE(mod1->extendedModules().size(), 0);
        QCOMPARE(mod2->extendedModules().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        QCATCH(obj->prependExtension(NULL), NullPointerException);
        obj->prependExtension(mod1);
        QCOMPARE(mod1->extendedModules().size(), 1);
        QCOMPARE(mod1->extendedModules()[0], obj);
        QCOMPARE(obj->extensions().size(), 1);
        QCOMPARE(obj->extensions()[0], mod1);
        QCATCH(obj->prependExtension(mod1), DuplicateException);
        obj->prependExtension(mod2);
        QCOMPARE(mod2->extendedModules().size(), 1);
        QCOMPARE(mod2->extendedModules()[0], obj);
        QCOMPARE(obj->extensions().size(), 2);
        QCOMPARE(obj->extensions()[0], mod2);
        QCOMPARE(obj->extensions()[1], mod1);
        QCATCH(obj->prependExtension(mod2), DuplicateException);
        delete obj;
        delete mod1;
        delete mod2;
    }

    void ModuleTest::removeExtension() {
        Module *mod1 = new Module;
        Module *mod2 = new Module;
        Module *obj = new Module;
        QCOMPARE(mod1->extendedModules().size(), 0);
        QCOMPARE(mod2->extendedModules().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        obj->addExtension(mod1);
        QCOMPARE(mod1->extendedModules().size(), 1);
        QCOMPARE(obj->extensions().size(), 1);
        QCATCH(obj->removeExtension(NULL), NullPointerException);
        QCATCH(obj->removeExtension(mod2), NotFoundException);
        obj->removeExtension(mod1);
        QCOMPARE(mod1->extendedModules().size(), 0);
        QCOMPARE(obj->extensions().size(), 0);
        obj->addExtension(mod1);
        obj->addExtension(mod2);
        QCOMPARE(obj->extensions().size(), 2);
        obj->removeExtension(mod1);
        QCOMPARE(obj->extensions().size(), 1);
        obj->removeExtension(mod2);
        QCOMPARE(obj->extensions().size(), 0);
        delete obj;
        delete mod1;
        delete mod2;
    }

    void ModuleTest::hasExtension() {
        Module *mod1 = new Module;
        Module *mod2 = new Module;
        Module *obj = new Module;
        obj->addExtension(mod1);
        QCATCH(obj->hasExtension(NULL), NullPointerException);
        QVERIFY(obj->hasExtension(mod1));
        QVERIFY(!obj->hasExtension(mod2));
        obj->addExtension(mod2);
        QVERIFY(obj->hasExtension(mod2));
        delete obj;
        delete mod1;
        delete mod2;
    }

    void ModuleTest::addParent() {
        Module *window1 = new Module;
        Module *window2 = new Module;
        Module *button1 = new Module;
        Module *button2 = new Module;
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
        ModuleList windows = button1->parents().values("button");
        QVERIFY(windows.contains(window1));
        QVERIFY(windows.contains(window2));
        delete(button1);
        delete(button2);
        delete(window1);
        delete(window2);
    }

    void ModuleTest::removeParent() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        window1->addParent("window1", world);
        Module *window2 = new Module;
        window2->addParent("window2", world);
        Module *button1 = new Module;
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

    void ModuleTest::directChild() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        QCATCH(world->directChild(""), ArgumentException);
        QCATCH(world->directChild("window"), NotFoundException);
        world->addDirectChild("window", window1);
        QCOMPARE(world->directChild("window"), window1);
    }

    void ModuleTest::hasDirectChild() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        QCATCH(world->hasDirectChild(""), ArgumentException);
        QVERIFY(!world->hasDirectChild("window"));
        world->addDirectChild("window", window1);
        QVERIFY(world->hasDirectChild("window"));
    }

    void ModuleTest::addDirectChild() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        Module *window2 = new Module;
        QCATCH(world->addDirectChild("window", NULL), NullPointerException);
        QCATCH(world->addDirectChild("", window1), ArgumentException);
        world->addDirectChild("window", window1);
        QCOMPARE(world->directChild("window"), window1);
        QCATCH(world->addDirectChild("window", window1), DuplicateException);
        world->addDirectChild("window2", window2);
        QCOMPARE(world->children().size(), 2);
        QCOMPARE(world->directChild("window2"), window2);
    }

    void ModuleTest::setDirectChild() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        Module *window2 = new Module;
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

    void ModuleTest::removeDirectChild() {
        Module w; Module *world = &w;
        Module *window1 = new Module;
        window1->addParent("window1", world);
        Module *button1 = new Module;
        QCATCH(window1->removeDirectChild(""), ArgumentException);
        QCATCH(window1->removeDirectChild("button"), NotFoundException);
        button1->addParent("button", window1);
        QCOMPARE(window1->children().size(), 1);
        window1->removeDirectChild("button");
        QCOMPARE(window1->children().size(), 0);
    }

    void ModuleTest::child() {
        Module w; Module *world = &w;
        world->addDirectChild("World", world);
        Module *Window = new Module;
        world->addDirectChild("Window", Window);
        Module *close = new Module;
        Window->addDirectChild("close", close);
        Module *window1 = Window->fork();
        Module *window2 = Window->fork();
        world->addDirectChild("win1", window1);
        world->addDirectChild("win2", window2);
        Module *button = new Module;
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
        Module *virtualClose = window1->child("close");
        QVERIFY(virtualClose->isVirtual());
        QVERIFY(virtualClose->isBasedOn(close));
        QCOMPARE(button->child("close"), virtualClose);
        Module *virtualButton = button->fork()->setIsVirtual(true);
        QCOMPARE(window1->forks().size(), 0);
        Module *virtualClose2 = virtualButton->child("close");
        QVERIFY(virtualClose2->isBasedOn(virtualClose));
        QCOMPARE(window1->forks().size(), 1);
        QVERIFY(virtualClose2->hasDirectParent(window1->forks().first()));
    }

    void ModuleTest::setChild() {
        Module w; Module *world = &w;
        Module *Person = world->addDirectChild("Person", new Module);
        Module *mvila = world->addDirectChild("mvila", Person->fork());
        Module *manu = new Module;
        Module *vila = new Module;
        QCATCH(mvila->setChild("", manu), ArgumentException);
        QCATCH(mvila->setChild("first_name", NULL), NullPointerException);
        QCATCH(mvila->setChild("first_name", manu), NotFoundException);
        Person->addDirectChild("first_name", new Module);
        Person->addDirectChild("last_name", new Module);
        QCOMPARE(mvila->setChild("first_name", manu), manu);
        QCOMPARE(mvila->setChild("last_name", vila), vila);
        QCOMPARE(mvila->directChild("first_name"), manu);
        QCOMPARE(mvila->directChild("last_name"), vila);
    }

    void ModuleTest::inspect() {
        Module w; Module *world = &w;
        Module *window = new Module;
        world->addDirectChild("window", window);
        Module *person = new Module;
        window->addDirectChild("person1", person);
        window->addDirectChild("person2", person);
        Module *first = new Module;
        Module *last = new Module;
        person->addDirectChild("first_name", first);
        person->addDirectChild("last_name", last);
        QCOMPARE(person->inspect(), person->uniqueHexID() + ": [person1, person2] => [last_name, first_name]");
    }
}
