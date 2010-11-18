#ifndef OPERATOR_H
#define OPERATOR_H

#include <QtCore/QString>

namespace Child {
    class Operator {
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

        const bool isNull() const { return(type == Null); }
    };
}

#endif // OPERATOR_H
