#ifndef PARSER_H
#define PARSER_H

#include "child/node.h"
#include "child/lexer.h"

namespace Child {
    class Parser {
    public:
        enum Rule {
            ListRule
        };

        Parser() : _position(0) { }

        const Token &token(const int i = 0) {
            loadToken(i);
            return(_lookahead.at(_position + i));
        }

        const Token::Type tokenType(const int i = 0) {
            return(token(i).type);
        }

        const QString tokenTypeName(const int i = 0) {
            return(token(i).typeName());
        }

        const QString tokenText(const int i = 0) {
            return(token(i).text());
        }

        void loadToken(const int i) {
            int n = (_position + i) - (_lookahead.size() - 1);
            if(n > 0) // out of tokens?
                for(int i = 1; i <= n; i++)
                    _lookahead.append(_lexer.nextToken());
        }

        void consume() {
            _position++;
            if(_position == _lookahead.size() && !isSpeculating()) {
                _position = 0;
                _lookahead.clear();
                clearRulesMemos();
            }
            loadToken(0);
        }

        void seek(const int position) {
            _position = position;
        }

        void pushPosition() {
            _positions.push(_position);
        }

        void popPosition() {
            seek(_positions.pop());
        }

        const bool isSpeculating() const {
            return(!_positions.isEmpty());
        }

        void memoize(const Rule rule, const int startPosition, const bool failed) {
            int stopPosition = failed ? -1 : _position;
            _rulesMemos[rule].insert(startPosition, stopPosition);
        }

        const bool isMemoized(const Rule rule) {
            if(!_rulesMemos.contains(rule)) return(false);
            _MemoHash &memos = _rulesMemos[rule];
            if(!memos.contains(_position)) return(false);
            int stopPosition = memos[_position];
            if(stopPosition == -1) throw(ParserException());
            seek(stopPosition);
            return(true);
        }

        void clearRulesMemos() {
            _rulesMemos.clear();
        }

        void openToken() {
            _openedTokens.push(token());
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
                throw(ParserException("expecting " + optionalText + Token::typeName(type) + ", found " + tokenTypeName()));
            }
        }

        void consumeNewline() {
             while(is(Token::Newline)) consume();
        }

        void consumeUselessNewline() {
            if(!is(Token::Newline)) return;
            if(_openedTokens.empty()) return;
            Token::Type type = _openedTokens.top().type;
            if(type == Token::LeftParenthesis || type == Token::LeftBracket)
               do consume(); while(is(Token::Newline));
        }

        const QString scanBody() {
            QString result;
            while(true) {
                consumeNewline();
                if(is(Token::Eof)) break;
                QString expression = scanExpression();
                if(!expression.isEmpty()) result.append(expression).append('\n');
            }
            return(result);
        }

        const QString scanExpression() {
            QString unary = scanUnaryExpressionChain();
            Operator op;
            if(isBinaryOperator(&op))
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
            if(result.isEmpty()) throw(ParserException("expecting UnaryExpression, found " + tokenTypeName()));
            return(result);
        }

        const bool isUnaryExpression() {
            return(isPrefixOperator() || isPrimaryExpression());
        }

        const QString scanUnaryExpression() {
            Operator op;
            if(isPrefixOperator(&op))
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
            Operator op;
            while(isPostfixOperator(&op))
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

        const bool isOperator(Operator::Type type = Operator::Null, Operator *foundOperator = NULL) {
            if(!is(Token::Operator)) return(false);
            if(type == Operator::Null) return(true);
            Operator op = _lexer.findOperator(tokenText(), type);
            if(op.isNull()) return(false);
            if(foundOperator) *foundOperator = op;
            return(true);
        }

        const bool isPrefixOperator(Operator *foundOperator = NULL) {
            return(isOperator(Operator::Prefix, foundOperator));
        }

        const QString scanPrefixOperator(const Operator &currentOp) {
            consume();
            consumeNewline();
            return(currentOp.name + "(" + scanUnaryExpression() + ")");
        }

        const bool isPostfixOperator(Operator *foundOperator = NULL) {
            return(isOperator(Operator::Postfix, foundOperator));
        }

        const QString scanPostfixOperator(const Operator &currentOp) {
            consume();
            consumeUselessNewline();
            return(currentOp.name);
        }

        const bool isBinaryOperator(Operator *foundOperator = NULL) {
            return(isOperator(Operator::Binary, foundOperator));
        }

        const QString scanBinaryOperator(QString leftHandSide, Operator currentOp, const short minPrecedence) {
            do {
                consume();
                consumeNewline();
                QString rightHandSide = scanUnaryExpressionChain();
                Operator nextOp;
                while(isBinaryOperator(&nextOp)) {
                    if(nextOp.associativity == Operator::NonAssociative && nextOp.precedence == currentOp.precedence)
                        qFatal("syntax error: chained non-associative operators with same precedence");
                    if(nextOp.precedence <= currentOp.precedence) break;
                    if(nextOp.associativity == Operator::RightAssociative && nextOp.precedence != currentOp.precedence) break;
                    rightHandSide = scanBinaryOperator(rightHandSide, nextOp, nextOp.precedence);
                }
                leftHandSide = "(" + leftHandSide + currentOp.name + rightHandSide + ")";
            } while(isBinaryOperator(&currentOp) && currentOp.precedence >= minPrecedence);
            return(leftHandSide);
        }

        void test() {
            _lexer = Lexer("a, b = b, a");
//            p(escapeTabsAndNewlines(_lexer.toString()).toUtf8());
            p(scanBody());
        }

    private:
        Lexer _lexer;
        QStack<int> _positions;
        QList<Token> _lookahead;
        int _position;
        QStack<Token> _openedTokens;
        typedef QHash<int, int> _MemoHash;
        QHash<Rule, _MemoHash> _rulesMemos;
    };
}

#endif // PARSER_H

//        void matchElement() {
//            if(is(Token::Name))
//                match(Token::Name);
//            else if(is(Token::LeftBracket))
//                matchList();
//            else
//                throw(ParserException("expecting Name or List, found " + tokenTypeName()));
//        }

//        void matchElements() {
//            matchElement();
//            while(is(Token::Comma)) {
//                match(Token::Comma);
//                matchElement();
//            }
//        }

//        void matchList() {
//            bool failed = false;
//            QString exceptionMessage;
//            int startPosition = _position;
//            if(isSpeculating() && isMemoized(ListRule)) return;
//            try { _matchList(); }
//            catch(ParserException e) { failed = true; exceptionMessage = e.message; }
//            if(isSpeculating()) memoize(ListRule, startPosition, failed);
//            if(failed) throw(ParserException(exceptionMessage));
//        }

//        void _matchList() {
//            p("parse list at: " + QString::number(_position));
//            match(Token::LeftBracket);
//            matchElements();
//            match(Token::RightBracket);
//        }

//        void matchAssignment() {
//            matchList();
//            match(Token::Operator, "=");
//            matchList();
//        }

//        void matchStatement() {
//            if(speculateList()) {
//                matchList(); match(Token::Eof);
//            } else if(speculateAssignment()) {
//                matchAssignment(); match(Token::Eof);
//            } else
//                throw(ParserException("expecting Statement, found " + tokenTypeName()));
//        }

//        const bool speculateList() {
//            bool success = true;
//            pushPosition();
//            try { matchList(); match(Token::Eof); }
//            catch(ParserException e) { success = false; }
//            popPosition();
//            return(success);
//        }

//        const bool speculateAssignment() {
//            bool success = true;
//            pushPosition();
//            try { matchAssignment(); match(Token::Eof); }
//            catch(ParserException e) { success = false; }
//            popPosition();
//            return(success);
//        }
