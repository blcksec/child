#ifndef CHILD_BUNCH_H
#define CHILD_BUNCH_H

#include "child/list.h"

CHILD_BEGIN

#define CHILD_BUNCH(ARGS...) new Bunch(Node::context()->child("Object", "Bunch"), ##ARGS)

class Bunch : public GenericList<Node *> {
    CHILD_DECLARE(Bunch, List);
public:
    explicit Bunch(const Node *origin) : GenericList<Node *>(origin, true) {}

    Bunch(const Node *origin, Node *value) : GenericList<Node *>(origin, value, true) {}

    Bunch(const Node *origin, Node *value1, Node *value2) :
        GenericList<Node *>(origin, value1, value2, true) {}

    Bunch(const Node *origin, Node *value1, Node *value2, Node *value3) :
        GenericList<Node *>(origin, value1, value2, value3, true) {}

    Bunch(const Bunch &other) : GenericList<Node *>(other) {}

    static void initRoot() { Object::root()->addChild("Bunch", root()); }

    virtual Bunch *fork() const {
        Bunch *bunch = new Bunch(this);
        bunch->initFork();
        return bunch;
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        return "[" + join(", ", "", "", debug, level) + "]";
    }
};

CHILD_END

#endif // CHILD_BUNCH_H
