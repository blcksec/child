#ifndef CHILD_TESTHELPER_H
#define CHILD_TESTHELPER_H

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
                         toString(t1->inspect()), toString(t2->inspect()), actual, expected, file, line);
}

}

#endif // CHILD_TESTHELPER_H
