#ifndef LEXER_H
#define LEXER_H

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
        void consumeSpaces();
        void consumeLineComment();
        void consumeBlockComment();
        void consumeEscapeSequence();
        void consumeEscapeSequenceNumber();

        void startToken() {
            _tokenPosition = _position;
            _tokenColumn = _column;
            _tokenLine = _line;
        }

        const Token finishToken(const Token::Type type) const {
            return(Token(type, tokenTextRef(), _tokenColumn, _tokenLine));
        }

        const QStringRef tokenTextRef() const {
            return(_source.midRef(_tokenPosition, _position - _tokenPosition));
        }

        const Token scan(const Token::Type type) { // Simple one char tokens
            startToken();
            consume();
            return(finishToken(type));
        }

        bool isName() const {
            return(_currentChar.isLetter() || _currentChar == '_');
        }

        const Token scanName();

        bool isOperator() const {
            return(_operatorStartChars.contains(_currentChar));
        }

        const Token scanOperator();
        const Token scanNumber();
        const Token scanCharacter();
        const Token scanText();

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
        int _column;
        int _line;
        QChar _previousChar;
        QChar _currentChar;
        QChar _nextChar;
        int _tokenPosition;
        int _tokenColumn;
        int _tokenLine;
        QMultiHash<QString, Operator> _operators;
        QString _operatorStartChars;
    };
}

#endif // LEXER_H
