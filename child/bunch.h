#ifndef CHILD_BUNCH_H
#define CHILD_BUNCH_H

#include "child/list.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Bunch, List);

#define CHILD_BUNCH(ARGS...) BunchPtr(new Bunch(Node::context()->child("Object", "Bunch"), ##ARGS))

class Bunch : public GenericList<BunchPtr, NodePtr> {
    CHILD_DECLARATION(Bunch, List);
public:
    Bunch(const NodePtr &origin, const NodeList &other = NodeList()) :
        GenericList<BunchPtr, NodePtr>(origin, other, true) {}

    Bunch(const Bunch &other) : GenericList<BunchPtr, NodePtr>(other) {}

    static void initRoot() { Object::root()->addChild("Bunch", root()); }

    virtual NodePtr fork() const { return BunchPtr(new Bunch(this))->initFork(); }
};

CHILD_PTR_DEFINITION(Bunch, List);

CHILD_END

#endif // CHILD_BUNCH_H
