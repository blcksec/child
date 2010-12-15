#include "child/language/operatortable.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(OperatorTable, List);

//    void OperatorTable::append(const QString &text, Operator::Type type, short precedence,
//                     Operator::Associativity associativity, const QString &name) {
//        if(!_operators) _operators = Dictionary::fork(this);
//        List *list;
//        if(operators()->hasKey(text))
//            list = List::as(operators()->get(text));
//        else {
//            list = List::fork(this);
//            operators()->set(text, list);
//        }
//        Operator *op = Operator::fork(this);
//        op->text = text;
//        op->type = type;
//        op->precedence = precedence;
//        op->associativity = associativity;
//        op->name = !name.isEmpty() ? name : text;
//        list->append(op);
//        if(!_firstChars.contains(text.at(0))) _firstChars.append(text.at(0));
//    }

//    Operator *OperatorTable::find(const QString &text, const Operator::Type type) const {
//        if(!operators()->hasKey(text)) return NULL;
//        List *list = List::as(operators()->get(text));
//        for(int i = 0; i < list->size(); i++) {
//            Operator *op = Operator::as(list->get(i));
//            if(op->type == type) return op;
//        }
//        return NULL;
//    }
}

CHILD_END
