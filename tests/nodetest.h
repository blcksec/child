#ifndef CHILD_NODE_TEST_H
#define CHILD_NODE_TEST_H

#include "node.h"

#include "tests/testbegin.h"

class NodeTest : public QObject {
    Q_OBJECT
private slots:
    void initialize();
    void setOrigin();
    void fork();
    void addExtension();
    void removeExtension();
    void hasExtension();
    void addChild();
    void setChild();
    void removeChild();
    void hasDirectChild();
    void hasChild();
};

#include "tests/testend.h"

#endif // CHILD_NODE_TEST_H
