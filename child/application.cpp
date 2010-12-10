#include "child/application.h"

CHILD_BEGIN

CHILD_DEFINITION(Application, Object);

void Application::initOperatorTable() {
    using namespace Language;

    OperatorTablePtr ops = CHILD_OPERATOR_TABLE();

    ops->append("++", Operator::Postfix, Operator::namePrecedence);
    ops->append("--", Operator::Postfix, Operator::namePrecedence);

    ops->append(".", Operator::Binary, Operator::namePrecedence);
    ops->append("...", Operator::Postfix, Operator::namePrecedence);

    ops->append("+", Operator::Prefix, 411, Operator::RightAssociative, "unary_plus");
    ops->append("-", Operator::Prefix, 411, Operator::RightAssociative, "unary_minus");
    ops->append("!", Operator::Prefix, 411, Operator::RightAssociative);

    ops->append("\\", Operator::Prefix, 421);
    ops->append("@", Operator::Prefix, 431);
    ops->append("#", Operator::Prefix, 411);
    ops->append("$", Operator::Prefix, 411);

    ops->append("+", Operator::Binary, 341);
    ops->append("-", Operator::Binary, 341);
    ops->append("*", Operator::Binary, 351);
    ops->append("/", Operator::Binary, 351);
    ops->append("%", Operator::Binary, 351);

    ops->append("&", Operator::Binary, 351);
    ops->append("|", Operator::Binary, 341);
    ops->append("^", Operator::Binary, 341);
    ops->append("<<", Operator::Binary, 351);
    ops->append(">>", Operator::Binary, 351);

    ops->append("&&", Operator::Binary, 321);
    ops->append("||", Operator::Binary, 311);

    ops->append("==", Operator::Binary, 331, Operator::NonAssociative);
    ops->append("!=", Operator::Binary, 331, Operator::NonAssociative);
    ops->append("<", Operator::Binary, 331, Operator::NonAssociative);
    ops->append(">", Operator::Binary, 331, Operator::NonAssociative);
    ops->append("<=", Operator::Binary, 331, Operator::NonAssociative);
    ops->append(">=", Operator::Binary, 331, Operator::NonAssociative);

    ops->append("..", Operator::Binary, 341);

    ops->append(",", Operator::Binary, 221);
    ops->append("->", Operator::Binary, 211);

    ops->append(":=", Operator::Binary, 111, Operator::RightAssociative);
    ops->append("=", Operator::Binary, 121, Operator::RightAssociative);
    ops->append("+=", Operator::Binary, 151, Operator::RightAssociative);
    ops->append("-=", Operator::Binary, 151, Operator::RightAssociative);
    ops->append("*=", Operator::Binary, 161, Operator::RightAssociative);
    ops->append("/=", Operator::Binary, 161, Operator::RightAssociative);
    ops->append("%=", Operator::Binary, 161, Operator::RightAssociative);
    ops->append("&=", Operator::Binary, 141, Operator::RightAssociative);
    ops->append("|=", Operator::Binary, 131, Operator::RightAssociative);
    ops->append("^=", Operator::Binary, 151, Operator::RightAssociative);
    ops->append("<<=", Operator::Binary, 161, Operator::RightAssociative);
    ops->append(">>=", Operator::Binary, 161, Operator::RightAssociative);

    ops->append(":", Operator::Binary, 11, Operator::NonAssociative, "", true);

    _operatorTable = ops;
    addChild("operatorTable", _operatorTable);
}

CHILD_END
