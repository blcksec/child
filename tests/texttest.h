#ifndef CHILD_TEXT_TEST_H
#define CHILD_TEXT_TEST_H

#include "child/text.h"

#include "tests/testbegin.h"

class TextTest : public QObject {
    Q_OBJECT
private slots:
    void initialize();
    void fork();
    void compare();
};

#include "tests/testend.h"

#endif // CHILD_TEXT_TEST_H
