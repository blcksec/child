#ifndef CHILD_TEST_BEGIN_H
#define CHILD_TEST_BEGIN_H

#include <QtTest/QtTest>

#include "child/exception.h"

CHILD_USE

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(EXCEPTION##Ptr &e) { /* qDebug() << e->report(); */ exceptionThrown = true; } \
    catch(ExceptionPtr &e) { QWARN(qPrintable(e->report())); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

namespace QTest {
    template <typename T1, typename T2>
    inline bool qCompare(const T1 &t1, const T2 &t2, const char *actual, const char *expected,
                        const char *file, int line) {
        return (t1 == t2)
            ? compare_helper(true, "COMPARE()", file, line)
            : compare_helper(false, "Compared values are not the same",
                             toString(t1->toString()), toString(t2->toString()), actual, expected, file, line);
    }
}

#define CHILD_TEST \
private: \
    HugeUnsignedInteger _initialNodeCount; \
private slots: \
    void init() { _initialNodeCount = Node::nodeCount(); } \
    void cleanup() { QVERIFY(Node::nodeCount() == _initialNodeCount); }

#endif // CHILD_TEST_BEGIN_H

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif
