#ifndef CHILD_PARSER_H
#define CHILD_PARSER_H

#include "child/boolean.h"
#include "child/number.h"
#include "child/character.h"
#include "child/text.h"
#include "child/message.h"
#include "child/block.h"
#include "child/language/lexer.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARSER(ARGS...) \
    new Language::Parser(Node::context()->child("Object", "Language", "Parser"), ##ARGS)

    class Parser : public Object {
        CHILD_DECLARE(Parser, Object);
    public:
        explicit Parser(Node *origin) : Object(origin), _lexer(NULL), _currentToken(NULL) {}

        static void initRoot() { Language::root()->addChild("Parser", root()); }

        CHILD_FORK_METHOD(Parser); // TODO

        Lexer *lexer() const {
            if(!_lexer)
                Parser::constCast(this)->_lexer = Lexer::cast(context()->child("lexer"));
            return _lexer;
        }

        Block *parse(const QString &source, const QString &resourceUrl = "") {
            lexer()->setSource(source);
            lexer()->setResourceUrl(resourceUrl);
            clearOpenedTokens();
            consume();
            return scanBlock();
        }

        const Token *token() const { return _currentToken; }
        const Token::Type tokenType() const { return _currentToken->type; }
        const QString tokenTypeName() const { return _currentToken->typeName(); }
        const QString tokenText() const { return _currentToken->text(); }

        void consume() { _currentToken = lexer()->nextToken(); }
        void openToken() { _openedTokens.push(tokenType()); } // TODO: replace with a RAII "TokenOpener" class
        void closeToken() { _openedTokens.pop(); }
        const Token::Type topToken() const { return _openedTokens.isEmpty() ? Token::Null : _openedTokens.top(); }
        void clearOpenedTokens() { _openedTokens.clear(); }

        const bool is(const Token::Type type, const QString &text = QString()) const {
            if(text.isNull())
                return tokenType() == type;
            else
                return tokenType() == type && tokenText() == text;
        }

        const QString match(const Token::Type type, const QString &text = QString()) {
            if(is(type, text)) {
                QString text(tokenText());
                consume();
                consumeUselessNewline();
                return text;
            } else {
                const QString optionalText = text.isNull() ? "" : "'" + text + "' ";
                throw parserException("expecting " + optionalText + Token::typeName(type) + ", found " + tokenTypeName());
            }
        }

        void consumeNewline() { while(is(Token::Newline) || is(Token::Semicolon)) consume(); }

        void consumeUselessNewline() {
            if(!is(Token::Newline)) return;
            Token::Type type = topToken();
            if(type == Token::LeftParenthesis || type == Token::LeftBracket)
               do consume(); while(is(Token::Newline));
        }

        Block *scanBlock(const Token::Type terminator = Token::Eof) {
            Block *block = CHILD_BLOCK();
            Section *section = NULL;
            consumeNewline();
            while(!is(terminator)) {
                Primitive *expression = scanExpression();
                if(expression)
                    if(Pair *pair = Pair::dynamicCast(expression->value())) {
                        section = CHILD_SECTION();
                        section->setLabel(Primitive::cast(pair->first()));
                        expression = Primitive::cast(pair->second());
                        block->append(section);
                    }
                if(expression) {
                    if(!section) {
                        section = CHILD_SECTION();
                        block->append(section);
                    }
                    section->append(expression);
                }
                consumeNewline();
            }
            return block;
        }

        Primitive *scanExpression() {
            Primitive *primitive = scanUnaryExpressionChain();
            if(!primitive) throw parserException("expecting UnaryExpression, found " + tokenTypeName());
            if(Operator *op = isBinaryOperator())
                return scanBinaryOperator(primitive, op, 0);
            return primitive;
        }

        Primitive *scanUnaryExpressionChain() {
            Primitive *primitive = NULL;
            while(isUnaryExpression()) {
                primitive = scanUnaryExpression(primitive);
                if(isBinaryOperator()) break;
            }
            return primitive;
        }

        const bool isUnaryExpression() const { return isPrefixOperator() || isPrimaryExpression(); }

        Primitive *scanUnaryExpression(Primitive *currentPrimitive) {
            if(Operator *op = isPrefixOperator())
                return scanPrefixOperator(currentPrimitive, op);
            else
                return scanPrimaryExpression(currentPrimitive);
        }

        const bool isPrimaryExpression() const { return isOperand(); }

        Primitive *scanPrimaryExpression(Primitive *currentPrimitive) {
            CHILD_PRIMITIVE_ADD(currentPrimitive, scanOperand());
            while(Operator *op = isPostfixOperator())
                scanPostfixOperator(currentPrimitive, op);
            return currentPrimitive;
        }

        const bool isOperand() const { return isName() || isLiteral() || isSubexpression() || isNestedBlock(); }

        Primitive *scanOperand() {
            if(isName()) {
                return scanName();
            } else if(isLiteral()) {
                return scanLiteral();
            } else if(isSubexpression()) {
                return scanSubexpression();
            } else if(isNestedBlock()) {
                return scanNestedBlock();
            }
            throw parserException("unimplemented operand");
        }

        const bool isName() const { return is(Token::Name); }

        Primitive *scanName() {
            Message *message = CHILD_MESSAGE(tokenText());
            Primitive *primitive = CHILD_PRIMITIVE(message);
            int begin = token()->sourceCodeRef.position();
            consume();
            if(is(Token::LeftParenthesis)) {
                openToken();
                consume();
                consumeNewline();
                if(!is(Token::RightParenthesis))
                    message->inputs()->append(scanExpression());
                closeToken();
                match(Token::RightParenthesis);
            }
//            if(isNestedBlock()) message->setBlock(Block::cast(scanNestedBlock()->value()));
            int end = token()->sourceCodeRef.position();
            primitive->setSourceCodeRef(lexer()->source().midRef(begin, end - begin));
            consumeUselessNewline();
            return primitive;
        }

        const bool isLiteral() const {
            Token::Type type = tokenType();
            return type == Token::Boolean || type == Token::Number ||
                   type == Token::Character || type == Token::Text;
        }

        Primitive *scanLiteral() {
            Node *value;
            QChar c;
            QString s;
            switch(tokenType()) {
            case Token::Boolean:
                value = CHILD_BOOLEAN(tokenText() == "yes" || tokenText() == "true");
                break;
            case Token::Number:
                value = CHILD_NUMBER(tokenText().toDouble());
                break;
            case Token::Character:
                c = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2)).at(0);
                value = CHILD_CHARACTER(c);
                break;
            case Token::Text:
                s = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2));
                value = CHILD_TEXT(s);
                break;
            default:
                throw parserException("unimplemented token");
            }
            Primitive *primitive = CHILD_PRIMITIVE(value, token()->sourceCodeRef);
            consume();
            consumeUselessNewline();
            return primitive;
        }

        const bool isSubexpression() const { return is(Token::LeftParenthesis); }

        Primitive *scanSubexpression() {
            Primitive *primitive = CHILD_PRIMITIVE();
            int begin = token()->sourceCodeRef.position();
            openToken();
            consume(); // Left parenthesis
            consumeNewline();
            primitive->setValue(scanExpression());
            int end = token()->sourceCodeRef.position() + 1;
            closeToken();
            match(Token::RightParenthesis);
            primitive->setSourceCodeRef(lexer()->source().midRef(begin, end - begin));
            return primitive;
        }

        const bool isNestedBlock() const { return is(Token::LeftBrace); }

        Primitive *scanNestedBlock() {
            Primitive *primitive = CHILD_PRIMITIVE();
            int begin = token()->sourceCodeRef.position();
            openToken();
            consume(); // Left brace
            consumeNewline();
            primitive->setValue(scanBlock(Token::RightBrace));
            int end = token()->sourceCodeRef.position() + 1;
            closeToken();
            match(Token::RightBrace);
            primitive->setSourceCodeRef(lexer()->source().midRef(begin, end - begin));
            return primitive;
        }

        Operator *isOperator(Operator::Type type) const {
            if(!is(Token::Operator)) return NULL;
            return lexer()->operatorTable()->has(tokenText(), type);
        }

        Operator *isPrefixOperator() const { return isOperator(Operator::Prefix); }

        Primitive *scanPrefixOperator(Primitive *currentPrimitive, Operator *currentOp) {
            Message *message = CHILD_MESSAGE(currentOp->name);
            Primitive *primitive = CHILD_PRIMITIVE(message, token()->sourceCodeRef);
            consume();
            consumeNewline();
            Primitive *chain = NULL;
            if(currentOp->isSpecial) {
                if(currentOp->name == "?:")
                    chain = scanExpression();
                else
                    throw parserException("unimplemented special operator");
            } else
                chain = scanUnaryExpression(NULL);
            CHILD_PRIMITIVE_ADD(currentPrimitive, chain);
            CHILD_PRIMITIVE_ADD(currentPrimitive, primitive);
            return currentPrimitive;
        }

        Operator *isPostfixOperator() const { return isOperator(Operator::Postfix); }

        Primitive *scanPostfixOperator(Primitive *currentPrimitive, Operator *currentOp) {
            if(currentOp->isSpecial) {
                if(currentOp->name == "...") {
                    if(!currentPrimitive) throw parserException("missing primitive before '...'");
                    Message *message = Message::dynamicCast(currentPrimitive->last()->value());
                    if(!message) throw parserException("missing message before '...'");
                    if(currentPrimitive->hasNext()) {
                        QString name = message->name();
                        Message *methodSignature = Message::dynamicCast(currentPrimitive->last()->previous()->value());
                        if(!methodSignature) throw parserException("missing method signature before '...'");
                        methodSignature->setCodeInputName(name);
                        currentPrimitive->last()->previous()->setNext(NULL);
                    } else
                        message->setIsVariadic(true);
                } else
                    throw parserException("unimplemented special operator");
            } else
                CHILD_PRIMITIVE_ADD(currentPrimitive,
                                    CHILD_PRIMITIVE(CHILD_MESSAGE(currentOp->name), token()->sourceCodeRef));
            consume();
            consumeUselessNewline();
            return currentPrimitive;
        }

        Operator *isBinaryOperator() const { return isOperator(Operator::Binary); }

        Primitive *scanBinaryOperator(Primitive *lhs, Operator *currentOp,
                                                 const short minPrecedence) {
            do {
                QStringRef sourceCodeRef = token()->sourceCodeRef;
                consume();
                if(currentOp->name != ":") consumeNewline();
                Primitive *rhs = scanUnaryExpressionChain();
                Operator *nextOp;
                while((nextOp = isBinaryOperator()) && (
                          operatorPrecedence(nextOp) > operatorPrecedence(currentOp) ||
                          (nextOp->associativity == Operator::RightAssociative &&
                           operatorPrecedence(nextOp) == operatorPrecedence(currentOp))
                          )) {
                    rhs = scanBinaryOperator(rhs, nextOp, operatorPrecedence(nextOp));
                }
                lhs = resolveBinaryOperator(lhs, currentOp, rhs, sourceCodeRef);
            } while((currentOp = isBinaryOperator()) && operatorPrecedence(currentOp) >= minPrecedence);
            return lhs;
        }

        Primitive *resolveBinaryOperator(Primitive *lhs,
                                         const Operator *op,
                                         Primitive *rhs,
                                         const QStringRef &sourceCodeRef) {
            if(op->isSpecial) {
                if(op->name == ":")
                    lhs = CHILD_PRIMITIVE(CHILD_PAIR(lhs, rhs), sourceCodeRef);
                else if(op->name == ",") {
                    checkRightHandSide(rhs);
                    Bunch *bunch = Bunch::dynamicCast(lhs->value());
                    if(!bunch)
                        lhs = CHILD_PRIMITIVE(CHILD_BUNCH(lhs, rhs), sourceCodeRef);
                    else
                        bunch->append(rhs);
                } else if(op->name == "->") {
                    checkRightHandSide(rhs);
                    Message *message = Message::dynamicCast(lhs->last()->value());
                    if(!message) throw parserException("message expected before '->'");
                    message->outputs()->append(rhs);
                } else
                    throw parserException("unimplemented special operator");
            } else {
                checkRightHandSide(rhs);
                if(op->useLHSAsReceiver) {
                    Message *message = CHILD_MESSAGE(op->name);
                    message->inputs()->append(rhs);
                    CHILD_PRIMITIVE_ADD(lhs, CHILD_PRIMITIVE(message, sourceCodeRef));
                } else {
                    Message *message = CHILD_MESSAGE(op->name);
                    message->inputs()->append(lhs->last());
                    message->inputs()->append(rhs);
                    if(lhs->hasNext())
                        lhs->last()->previous()->setNext(CHILD_PRIMITIVE(message, sourceCodeRef));
                    else
                        lhs = CHILD_PRIMITIVE(message, sourceCodeRef);
                }
            }
            return lhs;
        }

        void checkRightHandSide(const Primitive *rhs) {
            if(!rhs) throw parserException("expecting right-hand side expression, found " + tokenTypeName());
        }

        short operatorPrecedence(const Operator *op) const {
            short precedence = op->precedence;
            if(op->name == ",") {
                Token::Type type = topToken();
                if(type == Token::LeftParenthesis || type == Token::LeftBracket)
                    precedence = 1;
            }
            return precedence;
        }

        ParserException parserException(QString message) const {
            int column, line;
            computeColumnAndLineForPosition(lexer()->source(), token()->sourceCodeRef.position(), column, line);
            QString text = extractLine(lexer()->source(), line);
            if(!text.isEmpty()) {
                QString cursor = QString(" ").repeated(column - 1).append("^");
                message += "\n" + text + "\n" + cursor;
            }
            return ParserException(context()->child("ParserException"), message, lexer()->resourceName(), line);
        }
    private:
        Lexer *_lexer;
        Token *_currentToken;
        QStack<Token::Type> _openedTokens;
    };
}

CHILD_END

#endif // CHILD_PARSER_H
