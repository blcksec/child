#ifndef MODULETEST_H
#define MODULETEST_H

#include "tests/testhelper.h"
#include "child/module.h"

namespace Child {
    class ModuleTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialModuleCount;
    private slots:
        void init() {
            _initialModuleCount = Module::moduleCount();
        }

        void cleanup() {
            QVERIFY(Module::moduleCount() == _initialModuleCount);
        }

        void initialize();
        void setBaseModule();
        void unsetBaseModule();
        void isBasedOn();
        void fork();
        void addExtension();
        void prependExtension();
        void removeExtension();
        void hasExtension();
        void addParent();
        void removeParent();
        void directChild();
        void hasDirectChild();
        void addDirectChild();
        void setDirectChild();
        void removeDirectChild();
        void child();
        void setChild();
        void inspect();
    };
}

#endif // MODULETEST_H
