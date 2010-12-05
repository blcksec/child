#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Character, Object);

#define CHILD_CHARACTER(ARGS...) CharacterPtr(new Character(Node::findInContext("Object")->child("Character"), ##ARGS))

class Character : public Object {
    CHILD_DECLARATION(Character, Object);
public:
    Character(const NodePtr &origin, QChar value = QChar::Null) : Object(origin), _value(value) {}
    static void initRoot() { Object::root()->addChild("Character", root()); }
    virtual NodePtr fork() const { return NodePtr(new Character(NodePtr(this), _value)); }

    QChar value() const { return _value; }
    void setValue(QChar value) { _value = value; }

    virtual const QString inspect() const { return QString("'%1'").arg(value()); }
private:
    QChar _value;
};

CHILD_PTR_DEFINITION(Character, Object);

CHILD_END

#endif // CHILD_CHARACTER_H
