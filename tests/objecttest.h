#ifndef OBJECTTEST_H
#define OBJECTTEST_H

#include "tests/testhelper.h"
#include "child/object.h"

namespace Child {
    class ObjectTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialModuleCount;
    private slots:
        void initTestCase() {
            Object::initialize();
            _initialModuleCount = Module::moduleCount();
        }

        void cleanup() {
            QVERIFY(Module::moduleCount() == _initialModuleCount);
        }

        void initialize() {
            QVERIFY(Module::root()->hasDirectChild("Object"));
            QVERIFY(Module::root()->child("Object")->hasDirectModule(Module::root()));
        }
    };
}

#endif // OBJECTTEST_H
