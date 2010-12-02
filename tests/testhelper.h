#ifndef CHILD_TESTHELPER_H
#define CHILD_TESTHELPER_H

#include <QtTest/QtTest>

#include "child/exception.h"

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(EXCEPTION##Ptr &e) { qDebug() << e->report(); exceptionThrown = true; } \
    catch(ExceptionPtr &e) { QWARN(qPrintable(e->report())); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

#endif // CHILD_TESTHELPER_H
