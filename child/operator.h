#ifndef OPERATOR_H
#define OPERATOR_H

#include <QtCore/QString>

namespace Child {
    class Operator {
    public:
        enum Type { Unknown, Unary, Binary, LineComment, BlockComment };
        QString text;
        Type type;
        QString rightOperatorText;
        Operator(const QString &text = "", Type type = Unknown) :
                text(text), type(type) {}
    };
}

#endif // OPERATOR_H
