#include "operatortable.h"

namespace Child {
    OperatorTable *OperatorTable::_root = OperatorTable::root();

    OperatorTable *OperatorTable::root() {
        if(!_root) {
            _root = new OperatorTable;
            _root->setOrigin(Object::root());
            _root->addParent("OperatorTable", Object::root());
        }
        return(_root);
    }

    void OperatorTable::addOperator(const QString &text, Operator::Type type, short precedence,
                     Operator::Associativity associativity, const QString &name) {
        List *list;
        if(operators()->hasKey(text))
            list = CHILD_LIST(operators()->get(text));
        else {
            list = List::fork(this);
            operators()->set(text, list);
        }
        list->append(Operator::fork(this, text, type, precedence, associativity, name));
        if(!_startChars.contains(text.at(0))) _startChars.append(text.at(0));
    }

    Operator *OperatorTable::findOperator(const QString &text, const Operator::Type type) {
        if(!operators()->hasKey(text)) return(NULL);
        List *list = CHILD_LIST(operators()->get(text));
        for(int i = 0; i < list->size(); i++) {
            Operator *op = CHILD_OPERATOR(list->get(i));
            if(op->type == type) return(op);
        }
        return(NULL);
    }
}
