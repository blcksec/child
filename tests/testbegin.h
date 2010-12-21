#ifndef CHILD_TEST_BEGIN_H
#define CHILD_TEST_BEGIN_H

#include <QtTest/QtTest>

#include "child/exception.h"

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(EXCEPTION##Pointer &e) { /* qDebug() << e->report(); */ exceptionThrown = true; } \
    catch(ExceptionPointer &e) { QWARN(qPrintable(e->report())); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

#endif // CHILD_TEST_BEGIN_H

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif
