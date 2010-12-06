#ifndef CHILD_OPERATOR_H
#define CHILD_OPERATOR_H

#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Operator, Object);

    #define CHILD_OPERATOR(ARGS...) \
    OperatorPtr(new Operator(Node::findInContext("Object")->child("Language")->child("Operator"), ##ARGS))

    class Operator : public Object {
        CHILD_DECLARATION(Operator, Object);
    public:
        enum Type { Null, Prefix, Postfix, Binary };
        enum Associativity { LeftAssociative, RightAssociative, NonAssociative };
        static const short namePrecedence = 511;

        QString text;
        Type type;
        short precedence;
        Associativity associativity;
        QString name;

        Operator(const NodePtr &origin, const QString &text = "", Type type = Null, short precedence = 0,
                Associativity associativity = LeftAssociative, const QString &name = "") :
            Object(origin), text(text), type(type), precedence(precedence), associativity(associativity),name(name) {
            if(name.isEmpty()) this->name = text;
        }

        static void initRoot() { Language::root()->addChild("Operator", root()); }

        virtual NodePtr fork() const {
            return new Operator(this, text, type, precedence, associativity, name);
        }

        const bool isNull() const { return type == Null; }

        virtual const QString inspect() const { return QString("\"%1\"").arg(name); }
    };

    CHILD_PTR_DEFINITION(Operator, Object);
}

CHILD_END

#endif // CHILD_OPERATOR_H
