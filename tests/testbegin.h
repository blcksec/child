#ifndef CHILD_TEST_BEGIN_H
#define CHILD_TEST_BEGIN_H

#include <QtTest/QtTest>

#include "node/exception.h"

#define QCATCH(EXPRESSION, EXCEPTION) \
do { \
    bool exceptionThrown = false; \
    try { EXPRESSION; } \
    catch(const EXCEPTION &e) { /* qDebug() << e.report(); */ exceptionThrown = true; } \
    catch(const Exception &e) { QWARN(qPrintable(e.report())); } \
    if(!exceptionThrown) { QFAIL("'" #EXCEPTION "' has not been thrown"); } \
} while(false)

#endif // CHILD_TEST_BEGIN_H

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif
