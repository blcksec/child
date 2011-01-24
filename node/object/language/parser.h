#ifndef CHILD_LANGUAGE_PARSER_H
#define CHILD_LANGUAGE_PARSER_H

#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/character.h"
#include "node/object/text.h"
#include "node/object/message.h"
#include "node/object/language/block.h"
#include "node/object/language/lexer.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARSER(ARGS...) \
    new Language::Parser(context()->child("Object", "Language", "Parser"), ##ARGS)

    class Parser : public Object {
        CHILD_DECLARE(Parser, Object, Language);
    public:
        explicit Parser(Node *origin) : Object(origin), _lexer(NULL), _currentToken(NULL) {}

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Parser);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Parser); // TODO

        Lexer *lexer() const;

        Block *parse(const QString &source, const QString &resourceUrl = "");

        const Token *token() const { return _currentToken; }
        const Token::Type tokenType() const { return _currentToken->type; }
        const QString tokenTypeName() const { return _currentToken->typeName(); }
        const QString tokenText() const { return _currentToken->text(); }

        void consume() { _currentToken = lexer()->nextToken(); }
        void openToken() { _openedTokens.push(tokenType()); } // TODO: replace with a RAII "TokenOpener" class
        void closeToken() { _openedTokens.pop(); }
        const Token::Type topToken() const { return _openedTokens.isEmpty() ? Token::Null : _openedTokens.top(); }
        void clearOpenedTokens() { _openedTokens.clear(); }

        bool is(const Token::Type type, const QString &text = QString()) const;
        const QString match(const Token::Type type, const QString &text = QString());

        void consumeNewline();
        void consumeUselessNewline();

        Block *scanBlock(const Token::Type terminator = Token::Eof);
        Primitive *scanExpression();
        Primitive *scanUnaryExpressionChain();

        bool isUnaryExpression() const { return isPrefixOperator() || isPrimaryExpression(); }
        Primitive *scanUnaryExpression(Primitive *currentPrimitive);

        bool isPrimaryExpression() const { return isOperand(); }
        Primitive *scanPrimaryExpression(Primitive *currentPrimitive);

        bool isOperand() const {
            return isName() || isLiteral() || isCollection() || isSubexpression() || isNestedBlock();
        }
        Primitive *scanOperand();

        bool isName() const { return is(Token::Name); }
        Primitive *scanName();

        bool isLiteral() const;
        Primitive *scanLiteral();

        bool isCollection() const { return is(Token::LeftBracket); }
        Primitive *scanCollection();

        bool isSubexpression() const { return is(Token::LeftParenthesis); }
        Primitive *scanSubexpression();

        bool isNestedBlock() const { return is(Token::LeftBrace); }
        Primitive *scanNestedBlock();

        bool isCollectionAccessor() const { return is(Token::LeftBracket); }
        Primitive *scanCollectionAccessor(Primitive *currentPrimitive);

        Operator *isOperator(Operator::Type type) const;

        Operator *isPrefixOperator() const { return isOperator(Operator::Prefix); }
        Primitive *scanPrefixOperator(Primitive *currentPrimitive, Operator *currentOp);

        Operator *isPostfixOperator() const { return isOperator(Operator::Postfix); }
        Primitive *scanPostfixOperator(Primitive *currentPrimitive, Operator *currentOp);

        Operator *isBinaryOperator() const { return isOperator(Operator::Binary); }
        Primitive *scanBinaryOperator(Primitive *lhs, Operator *currentOp, const short minPrecedence);

        Primitive *resolveBinaryOperator(Primitive *lhs,
                                         const Operator *op,
                                         Primitive *rhs,
                                         const QStringRef &sourceCodeRef);

        void checkRightHandSide(const Primitive *rhs) {
            if(!rhs) throw parserException("expecting right-hand side expression, found " + tokenTypeName());
        }

        short operatorPrecedence(const Operator *op) const;

        ParserException parserException(QString message) const;
    private:
        Lexer *_lexer;
        Token *_currentToken;
        QStack<Token::Type> _openedTokens;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PARSER_H
