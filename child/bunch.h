#ifndef CHILD_BUNCH_H
#define CHILD_BUNCH_H

#include "child/list.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Bunch, List);

#define CHILD_BUNCH(ARGS...) BunchPtr(new Bunch(Node::context()->child("Object", "Bunch"), ##ARGS))

class Bunch : public GenericList<BunchPtr, NodePtr> {
    CHILD_DECLARATION(Bunch, List);
public:
    Bunch(const NodePtr &origin) : GenericList<BunchPtr, NodePtr>(origin, true) {}

    Bunch(const NodePtr &origin, const NodePtr &value) : GenericList<BunchPtr, NodePtr>(origin, value, true) {}

    Bunch(const NodePtr &origin, const NodePtr &value1, const NodePtr &value2) :
        GenericList<BunchPtr, NodePtr>(origin, value1, value2, true) {}

    Bunch(const NodePtr &origin, const NodePtr &value1, const NodePtr &value2, const NodePtr &value3) :
        GenericList<BunchPtr, NodePtr>(origin, value1, value2, value3, true) {}

    Bunch(const Bunch &other) : GenericList<BunchPtr, NodePtr>(other) {}

    static void initRoot() { Object::root()->addChild("Bunch", root()); }

    virtual NodePtr fork() const { return BunchPtr(new Bunch(this))->initFork(); }

    virtual const QString toString(bool debug = false) const { return "[" + join(", ", "", "", debug) + "]"; }
};

CHILD_PTR_DEFINITION(Bunch, List);

CHILD_END

#endif // CHILD_BUNCH_H
