#include "child/application.h"

namespace Child {
    Application *Application::_root = Application::root();

    Application *Application::root() {
        if(!_root) {
            _root = new Application;
            _root->setOrigin(Object::root());
            _root->addParent("Application", Object::root());
        }
        return(_root);
    }

    void Application::initOperatorTable() {
        _operatorTable = OperatorTable::fork(this);

        _operatorTable->addOperator("++", Operator::Postfix, Operator::namePrecedence);
        _operatorTable->addOperator("--", Operator::Postfix, Operator::namePrecedence);

        _operatorTable->addOperator(".", Operator::Binary, Operator::namePrecedence);
        _operatorTable->addOperator("...", Operator::Postfix, Operator::namePrecedence);

        _operatorTable->addOperator("+", Operator::Prefix, 411, Operator::RightAssociative, "unary_plus");
        _operatorTable->addOperator("-", Operator::Prefix, 411, Operator::RightAssociative, "unary_minus");
        _operatorTable->addOperator("!", Operator::Prefix, 411, Operator::RightAssociative);

        _operatorTable->addOperator("\\", Operator::Prefix, 421);
        _operatorTable->addOperator("@", Operator::Prefix, 431);
        _operatorTable->addOperator("#", Operator::Prefix, 411);
        _operatorTable->addOperator("$", Operator::Prefix, 411);

        _operatorTable->addOperator("+", Operator::Binary, 341);
        _operatorTable->addOperator("-", Operator::Binary, 341);
        _operatorTable->addOperator("*", Operator::Binary, 351);
        _operatorTable->addOperator("/", Operator::Binary, 351);
        _operatorTable->addOperator("%", Operator::Binary, 351);

        _operatorTable->addOperator("&", Operator::Binary, 351);
        _operatorTable->addOperator("|", Operator::Binary, 341);
        _operatorTable->addOperator("^", Operator::Binary, 341);
        _operatorTable->addOperator("<<", Operator::Binary, 351);
        _operatorTable->addOperator(">>", Operator::Binary, 351);

        _operatorTable->addOperator("&&", Operator::Binary, 321);
        _operatorTable->addOperator("||", Operator::Binary, 311);

        _operatorTable->addOperator("==", Operator::Binary, 331, Operator::NonAssociative);
        _operatorTable->addOperator("!=", Operator::Binary, 331, Operator::NonAssociative);
        _operatorTable->addOperator("<", Operator::Binary, 331, Operator::NonAssociative);
        _operatorTable->addOperator(">", Operator::Binary, 331, Operator::NonAssociative);
        _operatorTable->addOperator("<=", Operator::Binary, 331, Operator::NonAssociative);
        _operatorTable->addOperator(">=", Operator::Binary, 331, Operator::NonAssociative);

        _operatorTable->addOperator("..", Operator::Binary, 341);

        _operatorTable->addOperator(",", Operator::Binary, 221);
        _operatorTable->addOperator("->", Operator::Binary, 211);

        _operatorTable->addOperator(":=", Operator::Binary, 111, Operator::RightAssociative);
        _operatorTable->addOperator("=", Operator::Binary, 121, Operator::RightAssociative);
        _operatorTable->addOperator("+=", Operator::Binary, 151, Operator::RightAssociative);
        _operatorTable->addOperator("-=", Operator::Binary, 151, Operator::RightAssociative);
        _operatorTable->addOperator("*=", Operator::Binary, 161, Operator::RightAssociative);
        _operatorTable->addOperator("/=", Operator::Binary, 161, Operator::RightAssociative);
        _operatorTable->addOperator("%=", Operator::Binary, 161, Operator::RightAssociative);
        _operatorTable->addOperator("&=", Operator::Binary, 141, Operator::RightAssociative);
        _operatorTable->addOperator("|=", Operator::Binary, 131, Operator::RightAssociative);
        _operatorTable->addOperator("^=", Operator::Binary, 151, Operator::RightAssociative);
        _operatorTable->addOperator("<<=", Operator::Binary, 161, Operator::RightAssociative);
        _operatorTable->addOperator(">>=", Operator::Binary, 161, Operator::RightAssociative);
    }
}
