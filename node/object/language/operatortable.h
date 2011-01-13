#ifndef CHILD_LANGUAGE_OPERATOR_TABLE_H
#define CHILD_LANGUAGE_OPERATOR_TABLE_H

#include <QtCore/QSet>

#include "node/object/list.h"
#include "node/object/language/operator.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_OPERATOR_TABLE(ARGS...) \
    new Language::OperatorTable(context()->child("Object", "Language", "OperatorTable"), ##ARGS)

    class OperatorTable : public GenericList<Operator *> {
        CHILD_DECLARE(OperatorTable, List, Language);
    public:
        explicit OperatorTable(Node *origin) : GenericList<Operator *>(origin) {}

        CHILD_FORK_METHOD(OperatorTable);

        void append(const QString &text, Operator::Type type, short precedence,
                    Operator::Associativity associativity = Operator::LeftAssociative,
                    const bool useLHSAsReceiver = true, const bool isSpecial = false,
                    const QString &name = "") {
            GenericList<Operator *>::append(
                        CHILD_OPERATOR(text, type, precedence, associativity, useLHSAsReceiver, isSpecial, name));
        }

        bool has(const QString &text) const {
            return _texts.contains(text);
        }

        Operator *has(const QString &text, const Operator::Type type) const {
            return _textsAndTypes.value(QPair<QString, Operator::Type>(text, type));
        }

        bool hasOperatorStartingWith(const QChar c) const {
            return _firstChars.contains(c);
        }

        virtual void hasChanged();
    private:
        QSet<QString> _texts;
        QHash<QPair<QString, Operator::Type>, Operator *> _textsAndTypes;
        QString _firstChars;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_OPERATOR_TABLE_H
