#ifndef CHILD_LEXER_H
#define CHILD_LEXER_H

#include "child/node.h"
#include "child/token.h"
#include "child/operator.h"

namespace Child {
    class Lexer {
    public:
        Lexer(const QString &src = "", const QString &filename = "") {
            initOperators();
            setSource(src);
            setFilename(filename);
        }

        void initOperators();
        void addOperator(const QString &text, Operator::Type type, short precedence,
                         Operator::Associativity associativity = Operator::LeftAssociative,
                         const QString &name = "");
        Operator findOperator(const QString &text, const Operator::Type type) ;

        void setSource(const QString &source) { _source = source; rewind(); }
        const QString &source() const { return(_source); }

        void setFilename(const QString &filename) { _filename = filename;}
        const QString &filename() const { return(_filename); }

        void rewind();
        const Token nextToken();

        void consume();

        void startToken() {
            _tokenPosition = _position;
        }

        const Token finishToken(const Token::Type type) const {
            return(Token(type, tokenTextRef()));
        }

        const QStringRef tokenTextRef() const {
            return(_source.midRef(_tokenPosition, _position - _tokenPosition));
        }

        const Token scan(const Token::Type type) { // Simple one char tokens
            startToken();
            consume();
            return(finishToken(type));
        }

        bool isEof() const {
            return(_currentChar.isNull());
        }

        bool isSpace() const {
            return(_currentChar == ' ' || _currentChar == '\t');
        }

        void consumeSpaces() {
            do consume(); while(isSpace());
        }

        bool isNewline() const {
            return(_currentChar == '\r' || _currentChar == '\n');
        }

        const Token scanNewline() {
            startToken();
            do consume(); while(isNewline() || isSpace());
            return(finishToken(Token::Newline));
        }

        bool isLineComment() const {
            return(_currentChar == '/' && _nextChar == '/');
        }

        void consumeLineComment();

        bool isBlockComment() const {
            return(_currentChar == '/' && _nextChar == '*');
        }

        void consumeBlockComment();

        bool isName() const {
            return(_currentChar.isLetter() || _currentChar == '_');
        }

        const Token scanName();

        bool isOperator() const {
            return(_operatorStartChars.contains(_currentChar));
        }

        const Token scanOperator();

        bool isNumber() const {
            return(_currentChar.isNumber());
        }

        const Token scanNumber();

        const Token scanCharacter();
        const Token scanText();

        void consumeEscapeSequence();
        void consumeEscapeSequenceNumber();

        void throwError(const QString &message);

        const QString toString();

        void test() {
            setSource("[a, b] = [c, d]");
            p(escapeTabsAndNewlines(toString()).toUtf8());
        }

//        void test() {
//            setSource(readTextFile("../child/examples/lexertest.child"));
//            setFilename("lexertest.child");
//            p(escapeTabsAndNewlines(toString()).toUtf8());
//        }

//        void test() {
//            setSource(readTextFile("../child/examples/lexertest.child"));
//            setFilename("lexertest.child");
//            for(int i = 0; i < 1000; i++) {
//                while(nextToken().type != Token::Eof) {}
//                rewind();
//            }
//        }

    private:
        QString _source;
        QString _filename;
        int _position;
        QChar _previousChar;
        QChar _currentChar;
        QChar _nextChar;
        int _tokenPosition;
        QMultiHash<QString, Operator> _operators;
        QString _operatorStartChars;
    };
}

#endif // CHILD_LEXER_H
