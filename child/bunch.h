#ifndef CHILD_BUNCH_H
#define CHILD_BUNCH_H

#include "child/list.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Bunch, List);

#define CHILD_BUNCH(ARGS...) BunchPointer(new Bunch(Node::context()->child("Object", "Bunch"), ##ARGS))

class Bunch : public GenericList<BunchPointer, Pointer> {
    CHILD_DECLARE(Bunch, List);
public:
    explicit Bunch(const Pointer &origin) : GenericList<BunchPointer, Pointer>(origin, true) {}

    Bunch(const Pointer &origin, const Pointer &value) : GenericList<BunchPointer, Pointer>(origin, value, true) {}

    Bunch(const Pointer &origin, const Pointer &value1, const Pointer &value2) :
        GenericList<BunchPointer, Pointer>(origin, value1, value2, true) {}

    Bunch(const Pointer &origin, const Pointer &value1, const Pointer &value2, const Pointer &value3) :
        GenericList<BunchPointer, Pointer>(origin, value1, value2, value3, true) {}

    Bunch(const Bunch &other) : GenericList<BunchPointer, Pointer>(other) {}

    static void initRoot() { Object::root()->addChild("Bunch", root()); }

    virtual Pointer fork() const { return BunchPointer(new Bunch(this))->initFork(); }

    virtual QString toString(bool debug = false, short level = 0) const {
        return "[" + join(", ", "", "", debug, level) + "]";
    }
};

CHILD_POINTER_DEFINE(Bunch, List);

CHILD_END

#endif // CHILD_BUNCH_H
