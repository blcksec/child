#ifndef CHILD_RUNALLTESTS_H
#define CHILD_RUNALLTESTS_H

#include "tests/testhelper.h"
#include "tests/nodetest.h"
//#include "tests/objecttest.h"
//#include "tests/worldtest.h"
//#include "tests/texttest.h"
//#include "tests/listtest.h"
//#include "tests/dictionarytest.h"

namespace Child {
    static int testFailureCount;

    static void beginTests() {
        testFailureCount = 0;
        p("=================== Running all tests ===================");
    }

    static void test(QObject *testClass) {
        testFailureCount += QTest::qExec(testClass);
    }

    static void endTests() {
        if(testFailureCount == 0) {
            p("==================== All tests passed ===================");
        } else {
            p(QString("==================== %1 test(s) failed ===================").arg(testFailureCount));
        }
    }

    static void runAllTests() {
        beginTests();
        test(new NodeTest);
//        test(new ObjectTest);
//        test(new WorldTest);
//        test(new TextTest);
//        test(new ListTest);
//        test(new DictionaryTest);
        endTests();
    }
}

#endif // CHILD_RUNALLTESTS_H
