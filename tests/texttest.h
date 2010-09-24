#ifndef TEXTTEST_H
#define TEXTTEST_H

#include "tests/testhelper.h"
#include "child/world.h"

namespace Child {
    class TextTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialModuleCount;
    private slots:
        void initTestCase() {
            _initialModuleCount = Module::moduleCount();
        }

        void cleanup() {
            QVERIFY(Module::moduleCount() == _initialModuleCount);
        }

        void initialize() {
            QVERIFY(Module::root()->child("Object")->hasDirectChild("Text"));
        }

        void fork() {
            World w; World *world = &w;
            Text *t1 = CTEXT(world->child("Text"))->fork("Hello");
            world->addDirectChild("t1", t1);
            QVERIFY(t1->isBasedOn(world->child("Text")));
            QCOMPARE(t1->value(), QString("Hello"));
            Text *t1Up = CTEXT(t1->child("upcase"));
            world->addDirectChild("t1_up", t1Up);
            QCOMPARE(t1Up->value(), QString("HELLO"));
            Text *t2 = t1->fork();
            world->addDirectChild("t2", t2);
            QVERIFY(t2->isBasedOn(t1));
            QCOMPARE(t2->value(), QString("Hello"));
        }
    };
}

#endif // TEXTTEST_H
