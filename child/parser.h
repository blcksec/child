#ifndef CHILD_PARSER_H
#define CHILD_PARSER_H

#include "child/object.h"
#include "child/lexer.h"

#define CHILD_PARSER(EXPRESSION) static_cast<Parser *>(EXPRESSION)

namespace Child {
    class Parser : public Object {
    public:
        static Parser *root();
        static Parser *fork(Node *world) { return(CHILD_PARSER(world->child("Parser"))->fork()); }

        Parser() : _lexer(NULL), _currentToken(NULL) {}

        virtual ~Parser() {
            delete _currentToken;
        }

        virtual Parser *fork() { return(_fork(this)); }

        Lexer *lexer() const { return(_lexer); }
        void setLexer(Lexer *lexer) { _lexer = lexer; }

        const QString run() {
            consume();
            return(scanBlock());
        }

        const Token *token() const {
            return(_currentToken);
        }

        const Token::Type tokenType() const {
            return(_currentToken->type);
        }

        const QString tokenTypeName() const {
            return(_currentToken->typeName());
        }

        const QString tokenText() const {
            return(_currentToken->text());
        }

        void consume() {
            if(_currentToken) delete _currentToken;
            _currentToken = lexer()->nextToken();
        }

        void openToken() {
            _openedTokens.push(tokenType());
        }

        void closeToken() {
            _openedTokens.pop();
        }

        const bool is(const Token::Type type, const QString &text = QString()) {
            if(text.isNull())
                return(tokenType() == type);
            else
                return(tokenType() == type && tokenText() == text);
        }

        const QString match(const Token::Type type, const QString &text = QString()) {
            if(is(type, text)) {
                QString text(tokenText());
                consume();
                consumeUselessNewline();
                return(text);
            } else {
                const QString optionalText = text.isNull() ? "" : "'" + text + "' ";
                throwError("expecting " + optionalText + Token::typeName(type) + ", found " + tokenTypeName());
                return(""); // deadcode avoiding "control reaches end of non-void function" warning
            }
        }

        void consumeNewline() {
             while(is(Token::Newline)) consume();
        }

        void consumeUselessNewline() {
            if(!is(Token::Newline)) return;
            if(_openedTokens.empty()) return;
            Token::Type type = _openedTokens.top();
            if(type == Token::LeftParenthesis || type == Token::LeftBracket)
               do consume(); while(is(Token::Newline));
        }

        const QString scanBlock() {
            QString result;
            consumeNewline();
            while(!is(Token::Eof)) {
                result.append(scanSection());
            }
            return(result);
        }

        const QString scanSection() {
            QString result;
            if(is(Token::Label)) {
                result = tokenText();
                result.append('\n');
                consume();
                consumeNewline();
            }
            while(!(is(Token::Label) || is(Token::Eof))) {
                QString expression = scanExpression();
                consumeNewline();
                if(!expression.isEmpty()) result.append(expression).append('\n');
            }
            return(result);
        }

        const QString scanExpression() {
            QString unary = scanUnaryExpressionChain();
            if(Operator *op = isBinaryOperator())
                return(scanBinaryOperator(unary, op, 0));
            else
                return(unary);
        }

        const QString scanUnaryExpressionChain() {
            QString result;
            while(isUnaryExpression()) {
                result.append(scanUnaryExpression());
                if(isBinaryOperator()) break;
            }
            if(result.isEmpty()) throwError("expecting UnaryExpression, found " + tokenTypeName());
            return(result);
        }

        const bool isUnaryExpression() {
            return(isPrefixOperator() || isPrimaryExpression());
        }

        const QString scanUnaryExpression() {
            if(Operator *op = isPrefixOperator())
                return(scanPrefixOperator(op));
            else
                return(scanPrimaryExpression());
        }

        const bool isPrimaryExpression() {
            return(isOperand());
        }

        const QString scanPrimaryExpression() {
            QString result = scanOperand();
            if(is(Token::LeftParenthesis)) {
                openToken();
                consume();
                consumeNewline();
                result.append("(");
                if(!is(Token::RightParenthesis))
                    result.append(scanExpression());
                result.append(")");
                match(Token::RightParenthesis);
                closeToken();
            }
            while(Operator *op = isPostfixOperator())
                result.append(scanPostfixOperator(op));
            return(result);
        }

        const bool isOperand() {
            return(isName() || isLiteral() || isSubexpression());
        }

        const QString scanOperand() {
            if(isName()) {
                return(scanName());
            } else if(isLiteral()) {
                return(scanLiteral());
            } else {
                return(scanSubexpression());
            }
        }

        const bool isName() {
            return(is(Token::Name));
        }

        const QString scanName() {
            QString result = tokenText();
            consume();
            consumeUselessNewline();
            return(result);
        }

        const bool isLiteral() {
            Token::Type type = tokenType();
            return(type == Token::Boolean || type == Token::Number ||
                   type == Token::Character || type == Token::Text);
        }

        const QString scanLiteral() {
            QString result = tokenText();
            consume();
            consumeUselessNewline();
            return(result);
        }

        const bool isSubexpression() {
            return(is(Token::LeftParenthesis));
        }

        const QString scanSubexpression() {
            openToken();
            consume(); // Left parenthesis
            consumeNewline();
            QString result = scanExpression();
            match(Token::RightParenthesis);
            closeToken();
            return(result);
        }

        Operator *isOperator(Operator::Type type) {
            if(!is(Token::Operator)) return(NULL);
            return(lexer()->operatorTable()->findOperator(tokenText(), type));
        }

        Operator *isPrefixOperator() {
            return(isOperator(Operator::Prefix));
        }

        const QString scanPrefixOperator(Operator *currentOp) {
            consume();
            consumeNewline();
            return(currentOp->name + "(" + scanUnaryExpression() + ")");
        }

        Operator *isPostfixOperator() {
            return(isOperator(Operator::Postfix));
        }

        const QString scanPostfixOperator(Operator *currentOp) {
            consume();
            consumeUselessNewline();
            return(currentOp->name);
        }

        Operator *isBinaryOperator() {
            return(isOperator(Operator::Binary));
        }

        const QString scanBinaryOperator(QString leftHandSide, Operator *currentOp, const short minPrecedence) {
            do {
                consume();
                consumeNewline();
                QString rightHandSide = scanUnaryExpressionChain();
                while(Operator *nextOp = isBinaryOperator()) {
                    if(nextOp->associativity == Operator::NonAssociative && nextOp->precedence == currentOp->precedence)
                        qFatal("syntax error: chained non-associative operators with same precedence");
                    if(nextOp->precedence <= currentOp->precedence) break;
                    if(nextOp->associativity == Operator::RightAssociative && nextOp->precedence != currentOp->precedence) break;
                    rightHandSide = scanBinaryOperator(rightHandSide, nextOp, nextOp->precedence);
                }
                leftHandSide = "(" + leftHandSide + currentOp->name + rightHandSide + ")";
            } while((currentOp = isBinaryOperator()) && currentOp->precedence >= minPrecedence);
            return(leftHandSide);
        }

        void throwError(const QString &message) {
            QString report;
            if(!lexer()->filename().isEmpty()) report.append(QString("%1:").arg(lexer()->filename()));
            int column, line;
            computeColumnAndLineForPosition(*lexer()->source(), token()->sourceCodeRef.position(), column, line);
            report.append(QString("%1: %2").arg(line).arg(message));
            QString text = extractLine(*lexer()->source(), line);
            if(!text.isEmpty()) {
                QString cursor = QString(" ").repeated(column - 1).append("^");
                report.append(QString("\n%1\n%2").arg(text).arg(cursor));
            }
            throw(LexerException(report));
        }

    private:
        static Parser *_root;
        Lexer *_lexer;
        const Token *_currentToken;
        QStack<Token::Type> _openedTokens;
    };
}

#endif // CHILD_PARSER_H
