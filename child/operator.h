#ifndef OPERATOR_H
#define OPERATOR_H

#include <QtCore/QString>

namespace Child {
    class Operator {
    public:
        enum Type { Unknown, Unary, Binary };
        static const QString allowedChars;

        QString text;
        Type type;

        Operator(const QString &text = "", Type type = Unknown) :
                text(text), type(type) {}

//        void initOperators() {
//            addOperator("+", Operator::Binary);
//            addOperator("-", Operator::Binary);
//            addOperator("*", Operator::Binary);
//            addOperator("/", Operator::Binary);
//            addOperator("%", Operator::Binary);

//            addOperator("&", Operator::Binary);
//            addOperator("|", Operator::Binary);
//            addOperator("^", Operator::Binary);
//            addOperator("<<", Operator::Binary);
//            addOperator(">>", Operator::Binary);

//            addOperator("!", Operator::Unary);
//            addOperator("&&", Operator::Binary);
//            addOperator("||", Operator::Binary);

//            addOperator("==", Operator::Binary);
//            addOperator("!=", Operator::Binary);
//            addOperator("<", Operator::Binary);
//            addOperator(">", Operator::Binary);
//            addOperator("<=", Operator::Binary);
//            addOperator(">=", Operator::Binary);

//            addOperator(":=", Operator::Binary);
//            addOperator("=", Operator::Binary);
//            addOperator("+=", Operator::Binary);
//            addOperator("-=", Operator::Binary);
//            addOperator("*=", Operator::Binary);
//            addOperator("/=", Operator::Binary);
//            addOperator("%=", Operator::Binary);
//            addOperator("&=", Operator::Binary);
//            addOperator("|=", Operator::Binary);
//            addOperator("^=", Operator::Binary);
//            addOperator("<<=", Operator::Binary);
//            addOperator(">>=", Operator::Binary);

//            addOperator("++", Operator::Unary);
//            addOperator("--", Operator::Unary);

//            addOperator("@", Operator::Unary);
//            addOperator("#", Operator::Unary);
//            addOperator("$", Operator::Unary);

//            addOperator("<-", Operator::Binary);
//            addOperator("->", Operator::Binary);
//        }

//        void addOperator(const QString &text, Operator::Type type) {
//            operators.insert(text, Operator(text, type));
//        }
    };
}

#endif // OPERATOR_H
