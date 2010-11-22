#ifndef CHILD_PARSER_H
#define CHILD_PARSER_H

#include "child/object.h"
#include "child/lexer.h"
#include "child/block.h"
#include "child/section.h"
#include "child/primitivechain.h"
#include "child/primitive.h"
#include "child/message.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/character.h"
#include "child/text.h"

namespace Child {
    class Parser : public Object {
        CHILD_DECLARATION(Parser);
    public:
        Parser() : _lexer(NULL), _currentToken(NULL) {}

        virtual ~Parser() {
            delete _currentToken;
        }

        Lexer *lexer() const { return(_lexer); }
        void setLexer(Lexer *lexer) { _lexer = lexer; }

        Block *parse(const QString &source, const QString &resourceName = "") {
            lexer()->setSource(&source);
            lexer()->setResourceName(resourceName);
            clearOpenedTokens();
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

        const Token::Type topToken() const {
            if(_openedTokens.empty())
                return(Token::Null);
            else
                return(_openedTokens.top());
        }

        void clearOpenedTokens() {
            _openedTokens.clear();
        }

        const bool is(const Token::Type type, const QString &text = QString()) const {
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
            Token::Type type = topToken();
            if(type == Token::LeftParenthesis || type == Token::LeftBracket || type == Token::Label)
               do consume(); while(is(Token::Newline));
        }

        Block *scanBlock() {
            Block *block = Block::fork(this);
            consumeNewline();
            while(!is(Token::Eof))
                block->appendSection(scanSection());
            return(block);
        }

        Section *scanSection() {
            Section *section = Section::fork(this);
            if(is(Token::Label)) {
                section->setName(tokenText());
                consume();
                consumeNewline();
            }
            while(!(is(Token::Label) || is(Token::Eof))) {
                section->appendPrimitiveChain(scanExpression());
                consumeNewline();
            }
            return(section);
        }

        PrimitiveChain *scanExpression() {
            PrimitiveChain *primitiveChain = scanUnaryExpressionChain();
            if(Operator *op = isBinaryOperator())
                return(scanBinaryOperator(primitiveChain, op, 0));
            return(primitiveChain);
        }

        PrimitiveChain *scanUnaryExpressionChain() {
            PrimitiveChain *primitiveChain = PrimitiveChain::fork(this);
            if(is(Token::Label)) {
                Primitive *primitive = Primitive::fork(this);
                Message *message = Message::fork(this);
                primitive->setValue(message);
                primitive->setSourceCodeRef(token()->sourceCodeRef);
                message->setName(tokenText());
                openToken();
                consume();
                consumeNewline();
                message->appendInput("", scanExpression());
                closeToken();
                primitiveChain->append(primitive);
            }
            while(isUnaryExpression()) {
                scanUnaryExpression(primitiveChain);
                if(isBinaryOperator()) break;
            }
            if(primitiveChain->isEmpty()) throwError("expecting UnaryExpression, found " + tokenTypeName());
            return(primitiveChain);
        }

        const bool isUnaryExpression() const {
            return(isPrefixOperator() || isPrimaryExpression());
        }

        void scanUnaryExpression(PrimitiveChain *primitiveChain) {
            if(Operator *op = isPrefixOperator())
                scanPrefixOperator(primitiveChain, op);
            else
                scanPrimaryExpression(primitiveChain);
        }

        const bool isPrimaryExpression() const {
            return(isOperand());
        }

        void scanPrimaryExpression(PrimitiveChain *primitiveChain) {
            primitiveChain->append(scanOperand());
            while(Operator *op = isPostfixOperator())
                primitiveChain->append(scanPostfixOperator(op));
        }

        const bool isOperand() const {
            return(isName() || isLiteral() || isSubexpression());
        }

        Primitive *scanOperand() {
            if(isName()) {
                return(scanName());
            } else if(isLiteral()) {
                return(scanLiteral());
            } else {
                return(scanSubexpression());
            }
        }

        const bool isName() const {
            return(is(Token::Name));
        }

        Primitive *scanName() {
            Primitive *primitive = Primitive::fork(this);
            Message *message = Message::fork(this);
            primitive->setValue(message);
            message->setName(tokenText());
            int begin = token()->sourceCodeRef.position();
            consume();
            if(is(Token::LeftParenthesis)) {
                openToken();
                consume();
                consumeNewline();
                if(!is(Token::RightParenthesis)) {
                    PrimitiveChain *chain = scanExpression();
                    List *list = pairToList(chain);
                    message->appendInput(list);
                }
                closeToken();
                match(Token::RightParenthesis);
            }
            int end = token()->sourceCodeRef.position();
            primitive->setSourceCodeRef(lexer()->source()->midRef(begin, end - begin));
            consumeUselessNewline();
            return(primitive);
        }

        List *pairToList(PrimitiveChain *chain) { // TODO: simplify this code
            List *list = List::fork(this);
            if(chain->isNotEmpty()) {
                if(Message *msg = Message::is(chain->first(), ",")) {
                    delete list->append(pairToList(msg->firstInput()));
                    list->append(msg->secondInput());
                } else
                    list->append(chain);
            }
            return(list);
        }

        const bool isLiteral() const {
            Token::Type type = tokenType();
            return(type == Token::Boolean || type == Token::Number ||
                   type == Token::Character || type == Token::Text);
        }

        Primitive *scanLiteral() {
            Primitive *primitive = Primitive::fork(this);
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            QChar c;
            QString s;
            switch(tokenType()) {
            case Token::Boolean:
                primitive->setValue(Boolean::fork(this, tokenText() == "true"));
                break;
            case Token::Number:
                primitive->setValue(Number::fork(this, tokenText().toDouble()));
                break;
            case Token::Character:
                c = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2)).at(0);
                primitive->setValue(Character::fork(this, c));
                break;
            case Token::Text:
                s = Text::unescapeSequence(tokenText().mid(1, tokenText().size() - 2));
                primitive->setValue(Text::fork(this, s));
                break;
            default:
                throwError("unimplemented token!");
            }
            consume();
            consumeUselessNewline();
            return(primitive);
        }

        const bool isSubexpression() const {
            return(is(Token::LeftParenthesis));
        }

        Primitive *scanSubexpression() {
            Primitive *primitive = Primitive::fork(this);
            int begin = token()->sourceCodeRef.position();
            openToken();
            consume(); // Left parenthesis
            consumeNewline();
            primitive->setValue(scanExpression());
            int end = token()->sourceCodeRef.position() + 1;
            closeToken();
            match(Token::RightParenthesis);
            primitive->setSourceCodeRef(lexer()->source()->midRef(begin, end - begin));
            return(primitive);
        }

        Operator *isOperator(Operator::Type type) const {
            if(!is(Token::Operator)) return(NULL);
            return(lexer()->operatorTable()->find(tokenText(), type));
        }

        Operator *isPrefixOperator() const {
            return(isOperator(Operator::Prefix));
        }

        void scanPrefixOperator(PrimitiveChain *primitiveChain, Operator *currentOp) {
            Primitive *primitive = Primitive::fork(this);
            Message *message = Message::fork(this);
            primitive->setValue(message);
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            message->setName(currentOp->name);
            consume();
            consumeNewline();
            PrimitiveChain *unary = PrimitiveChain::fork(this);
            scanUnaryExpression(unary);
            message->appendInput("", unary);
            primitiveChain->append(primitive);
        }

        Operator *isPostfixOperator() const {
            return(isOperator(Operator::Postfix));
        }

        Primitive *scanPostfixOperator(Operator *currentOp) {
            Primitive *primitive = Primitive::fork(this);
            Message *message = Message::fork(this);
            primitive->setValue(message);
            primitive->setSourceCodeRef(token()->sourceCodeRef);
            message->setName(currentOp->name);
            consume();
            consumeUselessNewline();
            return(primitive);
        }

        Operator *isBinaryOperator() const {
            Operator *op = isOperator(Operator::Binary);
            if(op && op->name == "," && topToken() == Token::Label)
                return(NULL);
            else
                return(op);
        }

        PrimitiveChain *scanBinaryOperator(PrimitiveChain *leftHandSide, Operator *currentOp, const short minPrecedence) {
            do {
                Primitive *primitive = Primitive::fork(this);
                Message *message = Message::fork(this);
                primitive->setValue(message);
                primitive->setSourceCodeRef(token()->sourceCodeRef);
                message->setName(currentOp->name);
                consume();
                consumeNewline();
                PrimitiveChain *rightHandSide = scanUnaryExpressionChain();
                while(Operator *nextOp = isBinaryOperator()) {
                    if(nextOp->associativity == Operator::NonAssociative && nextOp->precedence == currentOp->precedence)
                        qFatal("syntax error: chained non-associative operators with same precedence");
                    if(nextOp->associativity != Operator::RightAssociative && nextOp->precedence <= currentOp->precedence) break;
                    if(nextOp->associativity == Operator::RightAssociative && nextOp->precedence != currentOp->precedence) break;
                    rightHandSide = scanBinaryOperator(rightHandSide, nextOp, nextOp->precedence);
                }
                message->appendInput("", leftHandSide);
                message->appendInput("", rightHandSide);
                leftHandSide = PrimitiveChain::fork(this);
                leftHandSide->append(primitive);
            } while((currentOp = isBinaryOperator()) && currentOp->precedence >= minPrecedence);
            return(leftHandSide);
        }

        void throwError(const QString &message) const {
            QString report;
            if(!lexer()->resourceName().isEmpty()) report.append(QString("%1:").arg(lexer()->resourceName()));
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
        Lexer *_lexer;
        const Token *_currentToken;
        QStack<Token::Type> _openedTokens;
    };
}

#endif // CHILD_PARSER_H
