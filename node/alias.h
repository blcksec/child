#ifndef CHILD_ALIAS_H
#define CHILD_ALIAS_H

#include <QtCore/QStringList>

#include "node.h"

CHILD_BEGIN

#define CHILD_ALIAS(ARGS...) new Alias(context()->child("Alias"), ##ARGS)

class Alias : public Node {
    CHILD_DECLARE(Alias, Node, Node);
public:
    explicit Alias(Node *origin, const QStringList &names = QStringList()) : Node(origin), _names(names) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Alias);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Alias, names());

    CHILD_DECLARE_NATIVE_METHOD(init);

    QStringList &names() const { return constCast(this)->_names; }
    void setNames(const QStringList &names) { _names = names; }

    bool isEmpty() const { return names().isEmpty(); }
    bool isNotEmpty() const { return !isEmpty(); }
private:
    QStringList _names;
};

CHILD_END

#endif // CHILD_ALIAS_H
