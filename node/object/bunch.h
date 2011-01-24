#ifndef CHILD_BUNCH_H
#define CHILD_BUNCH_H

#include "node/object/list.h"

CHILD_BEGIN

#define CHILD_BUNCH(ARGS...) new Bunch(context()->child("Object", "Bunch"), ##ARGS)

class Bunch : public GenericList<Node *> {
    CHILD_DECLARE(Bunch, List, Object);
public:
    explicit Bunch(Node *origin) : GenericList<Node *>(origin, true) {}

    Bunch(Node *origin, Node *value) : GenericList<Node *>(origin, value, true) {}

    Bunch(Node *origin, Node *value1, Node *value2) :
        GenericList<Node *>(origin, value1, value2, true) {}

    Bunch(Node *origin, Node *value1, Node *value2, Node *value3) :
        GenericList<Node *>(origin, value1, value2, value3, true) {}

    Bunch(const Bunch &other) : GenericList<Node *>(other) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Bunch);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Bunch);

    virtual QString toString(bool debug = false, short level = 0) const {
        return "[" + join(", ", "", "", debug, level) + "]";
    }
};

CHILD_END

#endif // CHILD_BUNCH_H
