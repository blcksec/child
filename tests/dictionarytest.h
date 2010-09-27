#ifndef DICTIONARYTEST_H
#define DICTIONARYTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/dictionary.h"
#include "child/world.h"

namespace Child {
    class DictionaryTest : public QObject
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
        void getAndSet();
    };
}

#endif // DICTIONARYTEST_H
