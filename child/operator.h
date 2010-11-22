#ifndef CHILD_OPERATOR_H
#define CHILD_OPERATOR_H

#include "child/object.h"

namespace Child {
    class Operator : public Object {
        CHILD_DECLARATION(Operator);
    public:
        enum Type { Null, Prefix, Postfix, Binary };
        enum Associativity { LeftAssociative, RightAssociative, NonAssociative };
        static const short namePrecedence = 511;

        QString text;
        Type type;
        short precedence;
        Associativity associativity;
        QString name;

        Operator(const QString &text = "", Type type = Null, short precedence = 0,
                 Associativity associativity = LeftAssociative, const QString &name = "")
                     : text(text), type(type), precedence(precedence), associativity(associativity), name(name) {
            if(name.isEmpty()) this->name = text;
        }

        virtual void initFork() {
            Operator *orig = Operator::as(origin());
            text = orig->text;
            type = orig->type;
            precedence = orig->precedence;
            associativity = orig->associativity;
            name = orig->name;
        }

        const bool isNull() const { return(type == Null); }

        virtual const QString inspect() const { return(QString("\"%1\"").arg(name)); }
    private:
    };
}

#endif // CHILD_OPERATOR_H
