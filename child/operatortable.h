#ifndef CHILD_OPERATORTABLE_H
#define CHILD_OPERATORTABLE_H

#include "child/object.h"
#include "child/list.h"
#include "child/dictionary.h"
#include "child/operator.h"

#define CHILD_OPERATORTABLE(EXPRESSION) static_cast<OperatorTable *>(EXPRESSION)

namespace Child {
    class OperatorTable : public Object {
    public:
        static OperatorTable *root();
        static OperatorTable *fork(Node *world) { return(CHILD_OPERATORTABLE(world->child("OperatorTable"))->fork()); }

        OperatorTable() : _operators(NULL) {}

        virtual ~OperatorTable() {
            delete _operators;
        }

        virtual OperatorTable *fork() { return(_fork(this)); }

        Dictionary *operators() const {
            return(_operators);
        }

        void append(const QString &text, Operator::Type type, short precedence,
                         Operator::Associativity associativity = Operator::LeftAssociative,
                         const QString &name = "");

        Operator *find(const QString &text, const Operator::Type type) const;

        bool hasOperator(const QString &text) const {
            return(operators()->hasKey(text));
        }

        bool hasOperatorStartingWith(const QChar c) const {
            return(_firstChars.contains(c));
        }

        virtual const QString inspect() const { return(operators()->inspect()); }
    private:
        static OperatorTable *_root;
        Dictionary *_operators;
        QString _firstChars;
    };
}

#endif // CHILD_OPERATORTABLE_H
