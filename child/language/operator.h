#ifndef CHILD_LANGUAGE_OPERATOR_H
#define CHILD_LANGUAGE_OPERATOR_H

#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(Operator, Object);

    #define CHILD_OPERATOR(ARGS...) \
    Language::OperatorPointer(new Language::Operator(Node::context()->child("Object", "Language", "Operator"), ##ARGS))

    class Operator : public Object {
        CHILD_DECLARE(Operator, Object);
    public:
        enum Type { Null, Prefix, Postfix, Binary };
        enum Associativity { LeftAssociative, RightAssociative, NonAssociative };
        static const short namePrecedence = 911;

        QString text;
        Type type;
        short precedence;
        Associativity associativity;
        bool useLHSAsReceiver;
        bool isSpecial;
        QString name;

        explicit Operator(const Pointer &origin, const QString &text = "", Type type = Null, short precedence = 0,
                Associativity associativity = LeftAssociative, const bool useLHSAsReceiver = true,
                 const bool isSpecial = false, const QString &name = "") :
            Object(origin), text(text), type(type), precedence(precedence), associativity(associativity),
            useLHSAsReceiver(useLHSAsReceiver), isSpecial(isSpecial), name(name) {
            if(name.isEmpty()) this->name = text;
        }

        static void initRoot() { Language::root()->addChild("Operator", root()); }

        virtual Pointer fork() const {
            return new Operator(this, text, type, precedence, associativity, useLHSAsReceiver, isSpecial, name);
        }

        const bool isNull() const { return type == Null; }

        virtual QString toString(bool debug = false, short level = 0) const {
            Q_UNUSED(debug);
            Q_UNUSED(level);
            return QString("\"%1\"").arg(name);
        }
    };

    CHILD_POINTER_DEFINE(Operator, Object);
}

CHILD_END

#endif // CHILD_LANGUAGE_OPERATOR_H
