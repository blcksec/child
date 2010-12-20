#include "child/application.h"

CHILD_BEGIN

CHILD_DEFINE(Application, Object);

void Application::initOperatorTable() {
    using namespace Language;

    OperatorTablePointer ops = CHILD_OPERATOR_TABLE();

    ops->append("++", Operator::Postfix, Operator::namePrecedence);
    ops->append("--", Operator::Postfix, Operator::namePrecedence);

    ops->append(".", Operator::Binary, Operator::namePrecedence);
    ops->append("...", Operator::Postfix, Operator::namePrecedence);

    ops->append("\\", Operator::Prefix, 521);
    ops->append("@", Operator::Prefix, 531);
    ops->append("#", Operator::Prefix, 511);
    ops->append("$", Operator::Prefix, 511);

    ops->append("+", Operator::Prefix, 511, Operator::RightAssociative, true, false, "unary_plus");
    ops->append("-", Operator::Prefix, 511, Operator::RightAssociative, true, false, "unary_minus");
    ops->append("!", Operator::Prefix, 511, Operator::RightAssociative);

    ops->append("+", Operator::Binary, 451);
    ops->append("-", Operator::Binary, 451);
    ops->append("*", Operator::Binary, 461);
    ops->append("/", Operator::Binary, 461);
    ops->append("%", Operator::Binary, 461);

    ops->append("&", Operator::Binary, 461);
    ops->append("|", Operator::Binary, 451);
    ops->append("^", Operator::Binary, 451);
    ops->append("<<", Operator::Binary, 461);
    ops->append(">>", Operator::Binary, 461);

    ops->append("..", Operator::Binary, 451);

    ops->append("<=>", Operator::Binary, 441);

    ops->append("==", Operator::Binary, 431, Operator::NonAssociative);
    ops->append("!=", Operator::Binary, 431, Operator::NonAssociative);
    ops->append("<", Operator::Binary, 431, Operator::NonAssociative);
    ops->append(">", Operator::Binary, 431, Operator::NonAssociative);
    ops->append("<=", Operator::Binary, 431, Operator::NonAssociative);
    ops->append(">=", Operator::Binary, 431, Operator::NonAssociative);

    ops->append("&&", Operator::Binary, 421);
    ops->append("||", Operator::Binary, 411);

    ops->append(",", Operator::Binary, 321, Operator::LeftAssociative, false, true);
    ops->append("->", Operator::Binary, 311, Operator::LeftAssociative, false, true);

    ops->append(":=", Operator::Binary, 211, Operator::RightAssociative, false);
    ops->append("=", Operator::Binary, 221, Operator::RightAssociative, false);
    ops->append("+=", Operator::Binary, 251, Operator::RightAssociative, false);
    ops->append("-=", Operator::Binary, 251, Operator::RightAssociative, false);
    ops->append("*=", Operator::Binary, 261, Operator::RightAssociative, false);
    ops->append("/=", Operator::Binary, 261, Operator::RightAssociative, false);
    ops->append("%=", Operator::Binary, 261, Operator::RightAssociative, false);
    ops->append("&=", Operator::Binary, 241, Operator::RightAssociative, false);
    ops->append("|=", Operator::Binary, 231, Operator::RightAssociative, false);
    ops->append("^=", Operator::Binary, 251, Operator::RightAssociative, false);
    ops->append("<<=", Operator::Binary, 261, Operator::RightAssociative, false);
    ops->append(">>=", Operator::Binary, 261, Operator::RightAssociative, false);

    ops->append("?:", Operator::Prefix, 121, Operator::NonAssociative, false, true);

    ops->append(":", Operator::Binary, 111, Operator::NonAssociative, false, true);

    _operatorTable = ops;
    addChild("operatorTable", _operatorTable);
}

CHILD_END
