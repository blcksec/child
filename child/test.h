#ifndef TEST_H
#define TEST_H

#include <QtTest/QtTest>

#include "child/module.h"
#include "child/text.h"

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(EXCEPTION) { exceptionThrown = true; } \
    catch(Exception &e) { QWARN(qPrintable(e.message)); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

namespace Child {
    class TestModule : public QObject
    {
        Q_OBJECT
    public:
        static void run() { TestModule test; QTest::qExec(&test); }

    private slots:
        void cleanup() {
            QVERIFY(Child::Module::moduleCount() == 0);
        }

        void addModule() {
            Module *mod1 = new Module;
            Module *mod2 = new Module;
            Module *obj = new Module;
            QCOMPARE(mod1->clones().size(), 0);
            QCOMPARE(mod2->clones().size(), 0);
            QCOMPARE(obj->modules().size(), 0);
            QCATCH(obj->addModule(NULL), NullPointerException);
            obj->addModule(mod1);
            QCOMPARE(mod1->clones().size(), 1);
            QCOMPARE(mod1->clones()[0], obj);
            QCOMPARE(obj->modules().size(), 1);
            QCOMPARE(obj->modules()[0], mod1);
            QCATCH(obj->addModule(mod1), DuplicateException);
            obj->addModule(mod2);
            QCOMPARE(mod2->clones().size(), 1);
            QCOMPARE(mod2->clones()[0], obj);
            QCOMPARE(obj->modules().size(), 2);
            QCOMPARE(obj->modules()[1], mod2);
            QCATCH(obj->addModule(mod2), DuplicateException);
            delete obj;
            delete mod1;
            delete mod2;
        }

        void prependModule() {
            Module *mod1 = new Module;
            Module *mod2 = new Module;
            Module *obj = new Module;
            QCOMPARE(mod1->clones().size(), 0);
            QCOMPARE(mod2->clones().size(), 0);
            QCOMPARE(obj->modules().size(), 0);
            QCATCH(obj->prependModule(NULL), NullPointerException);
            obj->prependModule(mod1);
            QCOMPARE(mod1->clones().size(), 1);
            QCOMPARE(mod1->clones()[0], obj);
            QCOMPARE(obj->modules().size(), 1);
            QCOMPARE(obj->modules()[0], mod1);
            QCATCH(obj->prependModule(mod1), DuplicateException);
            obj->prependModule(mod2);
            QCOMPARE(mod2->clones().size(), 1);
            QCOMPARE(mod2->clones()[0], obj);
            QCOMPARE(obj->modules().size(), 2);
            QCOMPARE(obj->modules()[0], mod2);
            QCOMPARE(obj->modules()[1], mod1);
            QCATCH(obj->prependModule(mod2), DuplicateException);
            delete obj;
            delete mod1;
            delete mod2;
        }

        void removeModule() {
            Module *mod1 = new Module;
            Module *mod2 = new Module;
            Module *obj = new Module;
            QCOMPARE(mod1->clones().size(), 0);
            QCOMPARE(mod2->clones().size(), 0);
            QCOMPARE(obj->modules().size(), 0);
            obj->addModule(mod1);
            QCOMPARE(mod1->clones().size(), 1);
            QCOMPARE(obj->modules().size(), 1);
            QCATCH(obj->removeModule(NULL), NullPointerException);
            QCATCH(obj->removeModule(mod2), NotFoundException);
            obj->removeModule(mod1);
            QCOMPARE(mod1->clones().size(), 0);
            QCOMPARE(obj->modules().size(), 0);
            obj->addModule(mod1);
            obj->addModule(mod2);
            QCOMPARE(obj->modules().size(), 2);
            obj->removeModule(mod1);
            QCOMPARE(obj->modules().size(), 1);
            obj->removeModule(mod2);
            QCOMPARE(obj->modules().size(), 0);
            delete obj;
            delete mod1;
            delete mod2;
        }

        void clone() {
            Module *obj1 = new Module;
            QCOMPARE(obj1->clones().size(), 0);
            Module *obj2 = obj1->clone();
            QCOMPARE(obj1->clones().size(), 1);
            QCOMPARE(obj1->clones()[0], obj2);
            QCOMPARE(obj2->modules().size(), 1);
            QCOMPARE(obj2->modules()[0], obj1);
            Module *obj3 = obj1->clone();
            QCOMPARE(obj1->clones().size(), 2);
            QCOMPARE(obj1->clones()[1], obj3);
            QCOMPARE(obj3->modules().size(), 1);
            QCOMPARE(obj3->modules()[0], obj1);
            delete obj3;
            delete obj2;
            delete obj1;

            Text *txt1 = new Text;
            Text *txt2 = txt1->clone();
            QCOMPARE(txt1->clones().size(), 1);
            QCOMPARE(txt1->clones()[0], txt2);
            QCOMPARE(txt2->modules().size(), 1);
            QCOMPARE(txt2->modules()[0], txt1);
            delete txt2;
            delete txt1;
        }

        void addParent() {
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

        void removeParent() {
            Module *world = new Module;
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
            delete world;
        }

        void child() {
            Module *world = new Module;
            Module *window1 = new Module;
            QCATCH(world->directChild(""), ArgumentException);
            QCATCH(world->directChild("window"), NotFoundException);
            world->addChild("window", window1);
            QCOMPARE(world->directChild("window"), window1);
            delete world;
        }

        void hasChild() {
            Module *world = new Module;
            Module *window1 = new Module;
            QCATCH(world->hasDirectChild(""), ArgumentException);
            QVERIFY(!world->hasDirectChild("window"));
            world->addChild("window", window1);
            QVERIFY(world->hasDirectChild("window"));
            delete world;
        }

        void addChild() {
            Module *world = new Module;
            Module *window1 = new Module;
            Module *window2 = new Module;
            QCATCH(world->addChild("window", NULL), NullPointerException);
            QCATCH(world->addChild("", window1), ArgumentException);
            world->addChild("window", window1);
            QCOMPARE(world->directChild("window"), window1);
            QCATCH(world->addChild("window", window1), DuplicateException);
            world->addChild("window2", window2);
            QCOMPARE(world->children().size(), 2);
            QCOMPARE(world->directChild("window2"), window2);
            delete world;
        }

        void setChild() {
            Module *world = new Module;
            Module *window1 = new Module;
            Module *window2 = new Module;
            QCATCH(world->setChild("window", NULL), NullPointerException);
            QCATCH(world->setChild("", window1), ArgumentException);
            QCATCH(world->setChild("window", window1), NotFoundException);
            world->addChild("window", window1);
            QCOMPARE(world->directChild("window"), window1);
            world->setChild("window", window1);
            QCOMPARE(world->children().size(), 1);
            QCOMPARE(world->directChild("window"), window1);
            world->setChild("window", window2);
            QCOMPARE(world->children().size(), 1);
            QCOMPARE(world->directChild("window"), window2);
            delete world;
        }

        void removeChild() {
            Module *world = new Module;
            Module *window1 = new Module;
            window1->addParent("window1", world);
            Module *button1 = new Module;
            QCATCH(window1->removeChild(""), ArgumentException);
            QCATCH(window1->removeChild("button"), NotFoundException);
            button1->addParent("button", window1);
            QCOMPARE(window1->children().size(), 1);
            window1->removeChild("button");
            QCOMPARE(window1->children().size(), 0);
            delete world;
        }

        void findChild() {
            Module *world = new Module;
            Module *Window = new Module;
            world->addChild("Window", Window);
            Module *close = new Module;
            Window->addChild("close", close);
            Module *window1 = Window->clone();
            Module *window2 = Window->clone();
            world->addChild("win1", window1);
            world->addChild("win2", window2);
            Module *button = new Module;
            window1->addChild("button", button);
            Module *own, *rcv;
            QCATCH(button->findChild("", own, rcv), ArgumentException);
            QVERIFY(!button->findChild("win3", own, rcv));
            QVERIFY(button->findChild("win2", own, rcv));
            QCOMPARE(own, world);
            QCOMPARE(rcv, world);
            QCOMPARE(own->directChild("win2"), window2);
            QVERIFY(button->findChild("close", own, rcv));
            QCOMPARE(own, Window);
            QCOMPARE(rcv, window1);
            QCOMPARE(own->directChild("close"), close);
            delete world;
        }

        void inspect() {
            Module *world = new Module;
            Module *window = new Module;
            world->addChild("window", window);
            Module *person = new Module;
            window->addChild("person1", person);
            window->addChild("person2", person);
            Module *first = new Module;
            Module *last = new Module;
            person->addChild("first_name", first);
            person->addChild("last_name", last);
            QCOMPARE(person->inspect(), person->uniqueHexID() + ": [person1, person2] => [last_name, first_name]");
            delete world;
        }
    };
}

#endif // TEST_H
