#ifndef CHILD_OBJECT_TEST_H
#define CHILD_OBJECT_TEST_H

#include "child/object.h"

#include "tests/testbegin.h"

class ObjectTest : public QObject {
    Q_OBJECT
    CHILD_TEST

    void initialize();
};

#include "tests/testend.h"

#endif // CHILD_OBJECT_TEST_H
