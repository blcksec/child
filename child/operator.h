#ifndef OPERATOR_H
#define OPERATOR_H

#include <QtCore/QString>

namespace Child {
    class Operator {
    public:
        enum Type { Unary, Binary, LineComment, BlockComment };

        QString text;
        Type type;

        Operator(const QString &text, Type type) : text(text), type(type) {}
    };
}

#endif // OPERATOR_H
