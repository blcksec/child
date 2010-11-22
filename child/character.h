#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "child/object.h"

namespace Child {
    class Character : public Object {
        CHILD_DECLARATION(Character);
    public:
        static Character *fork(Node *world, QChar value) {
            return(Character::fork(world)->setValue(value));
        }

        virtual void initFork() { _value = Character::as(origin())->_value; }

        QChar value() const { return(_value); }
        Character *setValue(QChar value) { _value = value; return(this); }

        virtual const QString inspect() const { return(QString("'%1'").arg(value())); }
    private:
        QChar _value;
    };
}

#endif // CHILD_CHARACTER_H
