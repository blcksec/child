#ifndef CHILD_TESTHELPER_H
#define CHILD_TESTHELPER_H

#include <QtTest/QtTest>

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(EXCEPTION) { exceptionThrown = true; } \
    catch(Exception &e) { QWARN(qPrintable(e.message)); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

#endif // CHILD_TESTHELPER_H
