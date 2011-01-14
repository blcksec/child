#include "node/object/list.h"

CHILD_BEGIN

CHILD_DEFINE(AbstractList, Object, Object);

void AbstractList::initRoot() {
}

CHILD_DEFINE(List, AbstractList, Object);

void List::initRoot() {
}

CHILD_DEFINE(VirtualList, AbstractList, Object);

void VirtualList::initRoot() {
}

CHILD_END
