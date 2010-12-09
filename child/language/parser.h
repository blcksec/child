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
    CHILD_PTR_DECLARATION(Parser, Object);

    #define CHILD_PARSER(ARGS...) \
    Language::ParserPtr(new Language::Parser(Node::context()->child("Object", "Language", "Parser"), ##ARGS))

    class Parser : public Object {
        CHILD_DECLARATION(Parser, Object);
    public:
        Parser(const NodePtr &origin) : Object(origin) {}
        static void initRoot() { Language::root()->addChild("Parser", root()); }
        virtual NodePtr fork() const { return new Parser(this); } // TODO

        LexerPtr lexer() const {
            if(!_lexer) const_cast<Parser *>(this)->_lexer = context()->child("lexer");
            return _lexer;
        }

        BlockPtr parse(const QString &source, const QString &resourceName = "") {
            lexer()->setSource(source);
            lexer()->setResourceName(resourceName);
            clearOpenedTokens();
            consume();
            return scanBlock();
        }

        const TokenPtr token() const {
            return _currentToken;
        }

        const Token::Type tokenType() const {
            return _currentToken->type;
        }

        const QString tokenTypeName() const {
            return _currentToken->typeName();
        }

        const QString tokenText() const {
            return _currentToken->text();
        }

        void consume() {
            _currentToken = lexer()->nextToken();
        }

        void openToken() {
            _openedTokens.push(tokenType());
        }

        void closeToken() {
            _openedTokens.pop();
        }

        const Token::Type topToken() const {
            if(_openedTokens.empty())
                return Token::Null;
            else
                return _openedTokens.top();
        }

        void clearOpenedTokens() {
            _openedTokens.clear();
        }

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
                throwError("expecting " + optionalText + Token::typeName(type) + ", found " + tokenTypeName());
                return ""; // deadcode avoiding "control reaches end of non-void function" warning
            }
        }

        void consumeNewline() {
             while(is(Token::Newline)) consume();
        }

        void consumeUselessNewline() {
            if(!is(Token::Newline)) return;
            Token::Type type = topToken();
            if(type == Token::LeftParenthesis || type == Token::LeftBracket || type == Token::Label)
               do consume(); while(is(Token::Newline));
        }

        BlockPtr scanBlock() {
            BlockPtr block = CHILD_BLOCK();
            consumeNewline();
            while(!is(Token::Eof)) block->append(scanSection());
            return block;
        }

        SectionPtr scanSection() {
            SectionPtr section = CHILD_SECTION();
            if(is(Token::Label)) {
                MessagePtr message = CHILD_MESSAGE();
                QString name = tokenText();
                name.chop(1);
                message->setName(name);
                PrimitivePtr primitive = CHILD_PRIMITIVE();
                primitive->setValue(message);
                primitive->setSourceCodeRef(token()->sourceCodeRef);
                PrimitiveChainPtr primitiveChain = CHILD_PRIMITIVE_CHAIN();
                primitiveChain->append(primitive);
                section->setLabel(primitiveChain);
                consume();
                consumeNewline();
            }
            while(!(is(Token::Label) || is(Token::Eof))) {
                section->append(scanExpression());
                consumeNewline();
            }
            return section;
        }

        PrimitiveChainPtr scanExpression() {
            PrimitiveChainPtr primitiveChain = scanUnaryExpressionChain();
            if(OperatorPtr op = isBinaryOperator())
                return scanBinaryOperator(primitiveChain, op, 0);
            return primitiveChain;
        }

        PrimitiveChainPtr scanUnaryExpressionChain() {
            PrimitiveChainPtr primitiveChain = CHILD_PRIMITIVE_CHAIN();
            if(is(Token::Label)) {
                PrimitivePtr primitive = CHILD_PRIMITIVE();
                MessagePtr message = CHILD_MESSAGE();
                primitive->setValue(message);
                primitive->setSourceCodeRef(token()->sourceCodeRef);
                message->setName(tokenText());
                openToken();
                consume();
                consumeNewline();
                message->inputs()->append(CHILD_ARGUMENT(PrimitiveChainPtr(), scanExpression()));
                closeToken();
                primitiveChain->append(primitive);
            }
            while(isUnaryExpression()) {
                scanUnaryExpression(primitiveChain);
                if(isBinaryOperator()) break;
            }
            if(primitiveChain->isEmpty()) throwError("expecting UnaryExpression, found " + tokenTypeName());
            return primitiveChain;
        }

        const bool isUnaryExpression() const {
            return isPrefixOperator() || isPrimaryExpression();
        }

        void scanUnaryExpression(PrimitiveChainPtr &primitiveChain) {
            if(OperatorPtr op = isPrefixOperator())
                scanPrefixOperator(primitiveChain, op);
            else
                scanPrimaryExpression(primitiveChain);
        }

        const bool isPrimaryExpression() const {
            return isOperand();
        }

        void scanPrimaryExpression(PrimitiveChainPtr &primitiveChain) {
            primitiveChain->append(scanOperand());
            while(OperatorPtr op = isPostfixOperator())
                primitiveChain->append(scanPostfixOperator(op));
        }

        const bool isOperand() const {
            return isName() || isLiteral() || isSubexpression();
        }

        PrimitivePtr scanOperand() {
            if(isName()) {
                return scanName();
            } else if(isLiteral()) {
                return scanLiteral();
            } else {
                return scanSubexpression();
            }
        }

        const bool isName() const {
            return is(Token::Name);
        }

        PrimitivePtr scanName() {
            PrimitivePtr primitive = CHILD_PRIMITIVE();
            MessagePtr message = CHILD_MESSAGE();
            primitive->setValue(message);
            message->setName(tokenText());
            int begin = token()->sourceCodeRef.position();
            consume();
            if(is(Token::LeftParenthesis)) {
                openToken();
                consume();
                consumeNewline();
                if(!is(Token::RightParenthesis)) {
                    PrimitiveChainPtr chain = scanExpression();
//                    List *list = pairToList(chain);
                    message->inputs()->append(CHILD_ARGUMENT(PrimitiveChainPtr(), chain));
                }
                closeToken();
                match(Token::RightParenthesis);
            }
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

        PrimitivePtr scanLiteral() {
            PrimitivePtr primitive = CHILD_PRIMITIVE();
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            QChar c;
            QString s;
            switch(tokenType()) {
            case Token::Boolean:
                primitive->setValue(CHILD_BOOLEAN(tokenText() == "true"));
                break;
            case Token::Number:
                primitive->setValue(CHILD_NUMBER(tokenText().toDouble()));
                break;
            case Token::Character:
                c = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2)).at(0);
                primitive->setValue(CHILD_CHARACTER(c));
                break;
            case Token::Text:
                s = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2));
                primitive->setValue(CHILD_TEXT(s));
                break;
            default:
                throwError("unimplemented token!");
            }
            consume();
            consumeUselessNewline();
            return primitive;
        }

        const bool isSubexpression() const {
            return is(Token::LeftParenthesis);
        }

        PrimitivePtr scanSubexpression() {
            PrimitivePtr primitive = CHILD_PRIMITIVE();
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

        OperatorPtr isOperator(Operator::Type type) const {
            if(!is(Token::Operator)) return NULL;
            return lexer()->operatorTable()->has(tokenText(), type);
        }

        OperatorPtr isPrefixOperator() const {
            return isOperator(Operator::Prefix);
        }

        void scanPrefixOperator(PrimitiveChainPtr &primitiveChain, OperatorPtr &currentOp) {
            PrimitivePtr primitive = CHILD_PRIMITIVE();
            MessagePtr message = CHILD_MESSAGE();
            primitive->setValue(message);
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            message->setName(currentOp->name);
            consume();
            consumeNewline();
            PrimitiveChainPtr unary = CHILD_PRIMITIVE_CHAIN();
            scanUnaryExpression(unary);
            message->inputs()->append(CHILD_ARGUMENT(PrimitiveChainPtr(), unary));
            primitiveChain->append(primitive);
        }

        OperatorPtr isPostfixOperator() const {
            return isOperator(Operator::Postfix);
        }

        PrimitivePtr scanPostfixOperator(OperatorPtr &currentOp) {
            PrimitivePtr primitive = CHILD_PRIMITIVE();
            MessagePtr message = CHILD_MESSAGE();
            primitive->setValue(message);
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            message->setName(currentOp->name);
            consume();
            consumeUselessNewline();
            return primitive;
        }

        OperatorPtr isBinaryOperator() const {
            OperatorPtr op = isOperator(Operator::Binary);
            if(op && op->name == "," && topToken() == Token::Label)
                return NULL;
            else
                return op;
        }

        PrimitiveChainPtr scanBinaryOperator(PrimitiveChainPtr leftHandSide, OperatorPtr &currentOp, const short minPrecedence) {
            do {
                PrimitivePtr primitive = CHILD_PRIMITIVE();
                MessagePtr message = CHILD_MESSAGE();
                primitive->setValue(message);
                primitive->setSourceCodeRef(token()->sourceCodeRef);
                message->setName(currentOp->name);
                consume();
                consumeNewline();
                PrimitiveChainPtr rightHandSide = scanUnaryExpressionChain();
                while(OperatorPtr nextOp = isBinaryOperator()) {
                    if(nextOp->associativity == Operator::NonAssociative && nextOp->precedence == currentOp->precedence)
                        qFatal("syntax error: chained non-associative operators with same precedence");
                    if(nextOp->associativity != Operator::RightAssociative && nextOp->precedence <= currentOp->precedence) break;
                    if(nextOp->associativity == Operator::RightAssociative && nextOp->precedence != currentOp->precedence) break;
                    rightHandSide = scanBinaryOperator(rightHandSide, nextOp, nextOp->precedence);
                }
                message->inputs()->append(CHILD_ARGUMENT(PrimitiveChainPtr(), leftHandSide));
                message->inputs()->append(CHILD_ARGUMENT(PrimitiveChainPtr(), rightHandSide));
                leftHandSide = CHILD_PRIMITIVE_CHAIN();
                leftHandSide->append(primitive);
            } while((currentOp = isBinaryOperator()) && currentOp->precedence >= minPrecedence);
            return leftHandSide;
        }

        void throwError(QString message) const {
            int column, line;
            computeColumnAndLineForPosition(lexer()->source(), token()->sourceCodeRef.position(), column, line);
            QString text = extractLine(lexer()->source(), line);
            if(!text.isEmpty()) {
                QString cursor = QString(" ").repeated(column - 1).append("^");
                message += "\n" + text + "\n" + cursor;
            }
            throw ParserExceptionPtr(new ParserException(context()->child("ParserException"),
                                                         message, lexer()->resourceName(), line));
        }
    private:
        LexerPtr _lexer;
        TokenPtr _currentToken;
        QStack<Token::Type> _openedTokens;
    };

    CHILD_PTR_DEFINITION(Parser, Object);
}

CHILD_END

#endif // CHILD_PARSER_H
