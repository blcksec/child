#ifndef CHILD_ALIAS_H
#define CHILD_ALIAS_H

#include "node.h"

CHILD_BEGIN

#define CHILD_ALIAS(ARGS...) new Alias(context()->child("Alias"), ##ARGS)

class Alias : public Node {
    CHILD_DECLARE(Alias, Node, Node);
public:
    explicit Alias(Node *origin, const QString &target = "") : Node(origin), _target(target) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Alias, target());

    CHILD_DECLARE_NATIVE_METHOD(init);

    QString target() const { return _target; }
    void setTarget(const QString &target) { _target = target; }
private:
    QString _target;
};

CHILD_END

#endif // CHILD_ALIAS_H
