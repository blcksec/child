#include "child/application.h"

namespace Child {
    CHILD_IMPLEMENTATION(Application, Object);

    void Application::initRoot() {
        // TODO
    }

    void Application::initOperatorTable() {
        OperatorTable *opTable = OperatorTable::fork(this);

        opTable->append("++", Operator::Postfix, Operator::namePrecedence);
        opTable->append("--", Operator::Postfix, Operator::namePrecedence);

        opTable->append(".", Operator::Binary, Operator::namePrecedence);
        opTable->append("...", Operator::Postfix, Operator::namePrecedence);

        opTable->append("+", Operator::Prefix, 411, Operator::RightAssociative, "unary_plus");
        opTable->append("-", Operator::Prefix, 411, Operator::RightAssociative, "unary_minus");
        opTable->append("!", Operator::Prefix, 411, Operator::RightAssociative);

        opTable->append("\\", Operator::Prefix, 421);
        opTable->append("@", Operator::Prefix, 431);
        opTable->append("#", Operator::Prefix, 411);
        opTable->append("$", Operator::Prefix, 411);

        opTable->append("+", Operator::Binary, 341);
        opTable->append("-", Operator::Binary, 341);
        opTable->append("*", Operator::Binary, 351);
        opTable->append("/", Operator::Binary, 351);
        opTable->append("%", Operator::Binary, 351);

        opTable->append("&", Operator::Binary, 351);
        opTable->append("|", Operator::Binary, 341);
        opTable->append("^", Operator::Binary, 341);
        opTable->append("<<", Operator::Binary, 351);
        opTable->append(">>", Operator::Binary, 351);

        opTable->append("&&", Operator::Binary, 321);
        opTable->append("||", Operator::Binary, 311);

        opTable->append("==", Operator::Binary, 331, Operator::NonAssociative);
        opTable->append("!=", Operator::Binary, 331, Operator::NonAssociative);
        opTable->append("<", Operator::Binary, 331, Operator::NonAssociative);
        opTable->append(">", Operator::Binary, 331, Operator::NonAssociative);
        opTable->append("<=", Operator::Binary, 331, Operator::NonAssociative);
        opTable->append(">=", Operator::Binary, 331, Operator::NonAssociative);

        opTable->append("..", Operator::Binary, 341);

        opTable->append(",", Operator::Binary, 221);
        opTable->append("->", Operator::Binary, 211);

        opTable->append(":=", Operator::Binary, 111, Operator::RightAssociative);
        opTable->append("=", Operator::Binary, 121, Operator::RightAssociative);
        opTable->append("+=", Operator::Binary, 151, Operator::RightAssociative);
        opTable->append("-=", Operator::Binary, 151, Operator::RightAssociative);
        opTable->append("*=", Operator::Binary, 161, Operator::RightAssociative);
        opTable->append("/=", Operator::Binary, 161, Operator::RightAssociative);
        opTable->append("%=", Operator::Binary, 161, Operator::RightAssociative);
        opTable->append("&=", Operator::Binary, 141, Operator::RightAssociative);
        opTable->append("|=", Operator::Binary, 131, Operator::RightAssociative);
        opTable->append("^=", Operator::Binary, 151, Operator::RightAssociative);
        opTable->append("<<=", Operator::Binary, 161, Operator::RightAssociative);
        opTable->append(">>=", Operator::Binary, 161, Operator::RightAssociative);

        _operatorTable = opTable;
    }
}
