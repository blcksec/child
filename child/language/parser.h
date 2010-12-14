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
    CHILD_POINTER_DECLARATION(Parser, Object);

    #define CHILD_PARSER(ARGS...) \
    Language::ParserPointer(new Language::Parser(Node::context()->child("Object", "Language", "Parser"), ##ARGS))

    class Parser : public Object {
        CHILD_DECLARATION(Parser, Object);
    public:
        Parser(const Pointer &origin) : Object(origin) {}
        static void initRoot() { Language::root()->addChild("Parser", root()); }
        virtual Pointer fork() const { return new Parser(this); } // TODO

        LexerPointer lexer() const {
            if(!_lexer) const_cast<Parser *>(this)->_lexer = context()->child("lexer");
            return _lexer;
        }

        BlockPointer parse(const QString &source, const QString &resourceUrl = "") {
            lexer()->setSource(source);
            lexer()->setResourceUrl(resourceUrl);
            clearOpenedTokens();
            consume();
            return scanBlock();
        }

        const TokenPointer token() const { return _currentToken; }
        const Token::Type tokenType() const { return _currentToken->type; }
        const QString tokenTypeName() const { return _currentToken->typeName(); }
        const QString tokenText() const { return _currentToken->text(); }

        void consume() { _currentToken = lexer()->nextToken(); }
        void openToken() { _openedTokens.push(tokenType()); }
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

        BlockPointer scanBlock(const Token::Type terminator = Token::Eof) {
            BlockPointer block = CHILD_BLOCK();
            SectionPointer section;
            consumeNewline();
            while(!is(terminator)) {
                PrimitiveChainPointer expression = scanExpression();
                if(PairPointer pair = PairPointer(expression->first()->value(), true)) {
                    section = CHILD_SECTION();
                    section->setLabel(pair->first());
                    expression = pair->second();
                    block->append(section);
                }
                if(expression->isNotEmpty()) {
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

        PrimitiveChainPointer scanExpression() {
            PrimitiveChainPointer primitiveChain = scanUnaryExpressionChain();
            if(primitiveChain->isEmpty()) throw parserException("expecting UnaryExpression, found " + tokenTypeName());
            if(OperatorPointer op = isBinaryOperator())
                return scanBinaryOperator(primitiveChain, op, 0);
            return primitiveChain;
        }

        PrimitiveChainPointer scanUnaryExpressionChain() {
            PrimitiveChainPointer primitiveChain = CHILD_PRIMITIVE_CHAIN();
            while(isUnaryExpression()) {
                scanUnaryExpression(primitiveChain);
                if(isBinaryOperator()) break;
            }
            return primitiveChain;
        }

        const bool isUnaryExpression() const { return isPrefixOperator() || isPrimaryExpression(); }

        void scanUnaryExpression(PrimitiveChainPointer &primitiveChain) {
            if(OperatorPointer op = isPrefixOperator())
                scanPrefixOperator(primitiveChain, op);
            else
                scanPrimaryExpression(primitiveChain);
        }

        const bool isPrimaryExpression() const { return isOperand(); }

        void scanPrimaryExpression(PrimitiveChainPointer &primitiveChain) {
            primitiveChain->append(scanOperand());
            while(OperatorPointer op = isPostfixOperator())
                primitiveChain->append(scanPostfixOperator(op));
        }

        const bool isOperand() const { return isName() || isLiteral() || isSubexpression() || isNestedBlock(); }

        PrimitivePointer scanOperand() {
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

        PrimitivePointer scanName() {
            MessagePointer message = CHILD_MESSAGE(tokenText());
            PrimitivePointer primitive = CHILD_PRIMITIVE(message);
            int begin = token()->sourceCodeRef.position();
            consume();
            if(is(Token::LeftParenthesis)) {
                openToken();
                consume();
                consumeNewline();
                if(!is(Token::RightParenthesis)) {
                    PrimitiveChainPointer chain = scanExpression();
//                    List *list = pairToList(chain);
                    message->inputs()->append(chain);
                }
                closeToken();
                match(Token::RightParenthesis);
            }
            if(isNestedBlock()) message->setBlock(scanNestedBlock()->value());
            int end = token()->sourceCodeRef.position();
            primitive->setSourceCodeRef(lexer()->source().midRef(begin, end - begin));
            consumeUselessNewline();
            return primitive;
        }

//        // flattenTuples
//        List *pairToList(PrimitiveChain *chain) { // TODO: simplify this code
//            List *list = List::fork(this);
//            if(chain->isNotEmpty()) {
//                if(Message *msg = Message::is(chain->first(), ",")) {
//                    delete list->append(pairToList(msg->firstInput()));
//                    list->append(msg->secondInput());
//                } else
//                    list->append(chain);
//            }
//            return list;
//        }

        const bool isLiteral() const {
            Token::Type type = tokenType();
            return(type == Token::Boolean || type == Token::Number ||
                   type == Token::Character || type == Token::Text);
        }

        PrimitivePointer scanLiteral() {
            Pointer value;
            QChar c;
            QString s;
            switch(tokenType()) {
            case Token::Boolean:
                value = CHILD_BOOLEAN(tokenText() == "true");
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
            PrimitivePointer primitive = CHILD_PRIMITIVE(value, token()->sourceCodeRef);
            consume();
            consumeUselessNewline();
            return primitive;
        }

        const bool isSubexpression() const { return is(Token::LeftParenthesis); }

        PrimitivePointer scanSubexpression() {
            PrimitivePointer primitive = CHILD_PRIMITIVE();
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

        PrimitivePointer scanNestedBlock() {
            PrimitivePointer primitive = CHILD_PRIMITIVE();
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

        OperatorPointer isOperator(Operator::Type type) const {
            if(!is(Token::Operator)) return NULL;
            return lexer()->operatorTable()->has(tokenText(), type);
        }

        OperatorPointer isPrefixOperator() const { return isOperator(Operator::Prefix); }

        void scanPrefixOperator(PrimitiveChainPointer &primitiveChain, OperatorPointer &currentOp) {
            MessagePointer message = CHILD_MESSAGE(currentOp->name);
            PrimitivePointer primitive = CHILD_PRIMITIVE(message, token()->sourceCodeRef);
            consume();
            consumeNewline();
            PrimitiveChainPointer unary = CHILD_PRIMITIVE_CHAIN();
            scanUnaryExpression(unary);
            message->inputs()->append(unary);
            primitiveChain->append(primitive);
        }

        OperatorPointer isPostfixOperator() const { return isOperator(Operator::Postfix); }

        PrimitivePointer scanPostfixOperator(OperatorPointer &currentOp) {
            PrimitivePointer primitive = CHILD_PRIMITIVE(CHILD_MESSAGE(currentOp->name), token()->sourceCodeRef);
            consume();
            consumeUselessNewline();
            return primitive;
        }

        OperatorPointer isBinaryOperator() const { return isOperator(Operator::Binary); }

        PrimitiveChainPointer scanBinaryOperator(PrimitiveChainPointer leftHandSide, OperatorPointer &currentOp, const short minPrecedence) {
            do {
                QStringRef sourceCodeRef = token()->sourceCodeRef;
                consume();
                if(currentOp->name != ":") consumeNewline();
                PrimitiveChainPointer rightHandSide = scanUnaryExpressionChain();
                OperatorPointer nextOp;
                while((nextOp = isBinaryOperator()) && (
                          operatorPrecedence(nextOp) > operatorPrecedence(currentOp) ||
                          (nextOp->associativity == Operator::RightAssociative &&
                           operatorPrecedence(nextOp) == operatorPrecedence(currentOp))
                          )) {
                    rightHandSide = scanBinaryOperator(rightHandSide, nextOp, operatorPrecedence(nextOp));
                }
                leftHandSide = resolveBinaryOperator(leftHandSide, currentOp, rightHandSide, sourceCodeRef);
            } while((currentOp = isBinaryOperator()) && operatorPrecedence(currentOp) >= minPrecedence);
            return leftHandSide;
        }

        PrimitiveChainPointer resolveBinaryOperator(PrimitiveChainPointer leftHandSide,
                                                    const OperatorPointer &op,
                                                    const PrimitiveChainPointer &rightHandSide,
                                                    const QStringRef &sourceCodeRef) {
            if(op->isSyntaxElement) {
                if(op->name == ":") {
                    PrimitivePointer primitive = CHILD_PRIMITIVE(CHILD_PAIR(leftHandSide, rightHandSide), sourceCodeRef);
                    leftHandSide = CHILD_PRIMITIVE_CHAIN(primitive);
                } else if(op->name == ",") {
                    BunchPointer bunch(leftHandSide->first()->value(), true);
                    if(!bunch) {
                        PrimitivePointer primitive = CHILD_PRIMITIVE(CHILD_BUNCH(leftHandSide, rightHandSide), sourceCodeRef);
                        leftHandSide = CHILD_PRIMITIVE_CHAIN(primitive);
                    } else {
                        bunch->append(rightHandSide);
                    }
                } else if(op->name == "->") {
                    MessagePointer message(leftHandSide->last()->value(), true);
                    if(!message) throw parserException("message expected before '->'");
                    message->outputs()->append(rightHandSide);
                } else
                    throw parserException("unimplemented syntax operator");
            } else {
                MessagePointer message = CHILD_MESSAGE(op->name);
                PrimitivePointer primitive = CHILD_PRIMITIVE(message, sourceCodeRef);
                message->inputs()->append(rightHandSide);
                leftHandSide = CHILD_PRIMITIVE_CHAIN(CHILD_PRIMITIVE(leftHandSide, QStringRef())); // TODO: proper sourceCodeRef
                leftHandSide->append(primitive);
            }
            return leftHandSide;
        }

        short operatorPrecedence(const OperatorPointer &op) const {
            short precedence = op->precedence;
            if(op->name == ",") {
                Token::Type type = topToken();
                if(type == Token::LeftParenthesis || type == Token::LeftBracket)
                    precedence = 1;
            }
            return precedence;
        }

        ParserExceptionPointer parserException(QString message) const {
            int column, line;
            computeColumnAndLineForPosition(lexer()->source(), token()->sourceCodeRef.position(), column, line);
            QString text = extractLine(lexer()->source(), line);
            if(!text.isEmpty()) {
                QString cursor = QString(" ").repeated(column - 1).append("^");
                message += "\n" + text + "\n" + cursor;
            }
            return new ParserException(context()->child("ParserException"),message, lexer()->resourceName(), line);
        }
    private:
        LexerPointer _lexer;
        TokenPointer _currentToken;
        QStack<Token::Type> _openedTokens;
    };

    CHILD_POINTER_DEFINITION(Parser, Object);
}

CHILD_END

#endif // CHILD_PARSER_H
