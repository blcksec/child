#ifndef CHILD_LIST_TEST_H
#define CHILD_LIST_TEST_H

#include "node/object/text.h"
#include "node/object/list.h"

#include "tests/testbegin.h"

class ListTest : public QObject {
    Q_OBJECT
private slots:
    void initialize();
    void insertGetAndSet();
    void remove();
    void clear();
};

#include "tests/testend.h"

#endif // CHILD_LIST_TEST_H
