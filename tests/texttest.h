#ifndef TEXTTEST_H
#define TEXTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/world.h"

namespace Child {
    class TextTest : public QObject
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
        void fork();
    };
}

#endif // TEXTTEST_H
