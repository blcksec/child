#ifndef CHILD_H
#define CHILD_H

#include <QtCore/QStack>

#include "child/node.h"

namespace Child {

void init();

const NodePtr context();
void pushContext(const NodePtr &node);
const NodePtr popContext();

static QStack<NodePtr> _contextStack;

} // namespace Child

#endif // CHILD_H
