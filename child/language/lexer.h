#ifndef CHILD_LEXER_H
#define CHILD_LEXER_H

#include "child/object.h"
#include "child/language.h"
#include "child/language/token.h"
#include "child/language/operatortable.h"

namespace Child {
    namespace Language {
        class Lexer : public Object {
            CHILD_DECLARATION(Lexer, Object, Language);
        public:
            Lexer() : _operatorTable(0), _source(0) {}

            OperatorTable *operatorTable() const { return _operatorTable; }
            void setOperatorTable(OperatorTable *table) { _operatorTable = table; }

            void setSource(const QString *source) { _source = source; rewind(); }
            const QString *source() const { return _source; }

            void setResourceName(const QString &name) { _resourceName = name;}
            const QString &resourceName() const { return _resourceName; }

            void rewind();
            const Token *nextToken();

            void consume();

            void startToken() {
                _tokenPosition = _position;
            }

            const Token *finishToken(const Token::Type type) {
                return Token::fork(this, type, tokenTextRef());
            }

            const QStringRef tokenTextRef() const {
                return _source->midRef(_tokenPosition, _position - _tokenPosition);
            }

            const Token *scan(const Token::Type type) { // Simple one char tokens
                startToken();
                consume();
                return finishToken(type);
            }

            bool isEof() const {
                return _currentChar.isNull();
            }

            bool isSpace() const {
                return _currentChar == ' ' || _currentChar == '\t';
            }

            void consumeSpaces() {
                do consume(); while(isSpace());
            }

            bool isNewline() const {
                return _currentChar == '\r' || _currentChar == '\n';
            }

            const Token *scanNewline() {
                startToken();
                do consume(); while(isNewline() || isSpace());
                return finishToken(Token::Newline);
            }

            bool isLineComment() const {
                return _currentChar == '/' && _nextChar == '/';
            }

            void consumeLineComment();

            bool isBlockComment() const {
                return _currentChar == '/' && _nextChar == '*';
            }

            void consumeBlockComment();

            bool isName() const {
                return _currentChar.isLetter() || _currentChar == '_';
            }

            const Token *scanName();

            bool isOperator() const {
                return operatorTable()->hasOperatorStartingWith(_currentChar);
            }

            const Token *scanOperator();

            bool isNumber() const {
                return _currentChar.isNumber();
            }

            const Token *scanNumber();

            const Token *scanCharacter();
            const Token *scanText();

            void consumeEscapeSequence();
            void consumeEscapeSequenceNumber();

            void throwError(const QString &message);

            const QString toString();

            void test() {
    //            setSource("[a, b] = [c, d]");
    //            p(escapeTabsAndNewlines(toString()).toUtf8());
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
            OperatorTable *_operatorTable;
            const QString *_source;
            QString _resourceName;
            int _position;
            QChar _previousChar;
            QChar _currentChar;
            QChar _nextChar;
            int _tokenPosition;
        };
    }
}

#endif // CHILD_LEXER_H
