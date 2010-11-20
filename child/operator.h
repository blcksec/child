#ifndef CHILD_OPERATOR_H
#define CHILD_OPERATOR_H

#include "child/object.h"

#define CHILD_OPERATOR(EXPRESSION) static_cast<Operator *>(EXPRESSION)

namespace Child {
    class Operator : public Object {
    public:
        enum Type { Null, Prefix, Postfix, Binary };
        enum Associativity { LeftAssociative, RightAssociative, NonAssociative };
        static const short namePrecedence = 511;

        static Operator *root();
        static Operator *fork(Node *world) { return(CHILD_OPERATOR(world->child("Operator"))->fork()); }
        static Operator *fork(Node *world, const QString &text, Type type, short precedence,
                              Associativity associativity,const QString &name) {
            return(CHILD_OPERATOR(world->child("Operator"))->fork(text, type, precedence, associativity, name));
        }

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

        virtual Operator *fork() {
            Operator *op = _fork(this);
            op->text = text;
            op->type = type;
            op->precedence = precedence;
            op->associativity = associativity;
            op->name = name;
            return(op);
        }

        virtual Operator *fork(const QString &text, Type type, short precedence,
                               Associativity associativity,const QString &name) {
            Operator *op = _fork(this);
            op->text = text;
            op->type = type;
            op->precedence = precedence;
            op->associativity = associativity;
            op->name = !name.isEmpty() ? name : text;
            return(op);
        }

        const bool isNull() const { return(type == Null); }

        virtual const QString inspect() const { return(QString("\"%1\"").arg(name)); }
    private:
        static Operator *_root;
    };
}

#endif // CHILD_OPERATOR_H
