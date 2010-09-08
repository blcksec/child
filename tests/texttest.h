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
            QVERIFY(t1->hasDirectModule(world->child("Text")));
            QCOMPARE(t1->value(), QString("Hello"));
            QCOMPARE(CTEXT(t1->child("upcase"))->value(), QString("HELLO"));
            Text *t2 = t1->fork();
            QVERIFY(t2->hasDirectModule(t1));
            QCOMPARE(t2->value(), QString("Hello"));
        }
    };
}

#endif // TEXTTEST_H
