#ifndef LEXER_H
#define LEXER_H

#include "child/node.h"
#include "child/token.h"

namespace Child {
    class Lexer {
    public:
        enum TokenType {
            Eof,
            Name,
            Operator,
            Number,
            Text
        };
        static const char *tokenTypesName[];
        static const QString allowedCharsInNames;
        static const QString firstCharsInOperators;

        Lexer(const QString &src = "") { setSource(src); }

        void setSource(const QString &source) {
            _source = source;
            _cursor = -1;
            consume();
        }

        const QString &source() const { return(_source); }

        const Token nextToken() {
            while(!_currentChar.isNull()) {
                if(_currentChar.isSpace()) {
                    consumeSpaces();
                } else if(isName()) {
                    return(nameToken());
                } else if(isOperator()) {
                    return(operatorToken());
                } else if(_currentChar.isNumber()) {
                    return(numberToken());
                } else if(_currentChar == '"') {
                    return(textToken());
                } else {
                    throw LexerException(QString("Invalid character: '%1'").arg(_currentChar));
                }
            }
            return(Token(Eof, "<EOF>"));
        }

        void consume() {
            _cursor++;
            if(_cursor < _source.length()) {
                _currentChar = _source.at(_cursor);
            } else {
                _currentChar = QChar::Null;
            }
        }

        void consumeSpaces() {
            do { consume(); } while(_currentChar.isSpace());
        }

        bool isName() {
            return(_currentChar.isLetter() || _currentChar == '_');
        }

        const Token nameToken() {
            QString text;
            do {
                text.append(_currentChar);
                consume();
            } while(_currentChar.isLetterOrNumber() || allowedCharsInNames.contains(_currentChar));
            return(Token(Name, text));
        }

        bool isOperator() {
            return(firstCharsInOperators.contains(_currentChar));
        }

        const Token operatorToken() { // TODO: Add support for operators with more than one character
            QString text;
            text.append(_currentChar);
            consume();
            return(Token(Operator, text));
        }

        const Token numberToken() {
            QString text(_currentChar);
            consume();
            bool dotFound = false;
            bool eFound = false;
            bool xFound = false;
            bool bFound = false;
            bool oFound = (text == "0");
            bool numberExpected = false;
            while(true) {
                if(_currentChar.isNumber()) {
                    if(bFound && !(_currentChar == '0' || _currentChar == '1'))
                        throw LexerException("A binary number can only contain 0 or 1");
                    if(oFound && !QString("01234567").contains(_currentChar))
                        throw LexerException("An octal number can only contain digits from 0 to 7");
                    numberExpected = false;
                } else if(_currentChar == '.') {
                    if(dotFound) throw LexerException("A number cannot contain more than one dot");
                    if(eFound) throw LexerException("The exponential part of a number cannot contain a dot");
                    if(xFound) throw LexerException("An hexadecimal number cannot contain a dot");
                    if(bFound) throw LexerException("A binary number cannot contain a dot");
                    if(text == "0") oFound = false;
                    if(oFound) throw LexerException("An octal number cannot contain a dot");
                    dotFound = true;
                    numberExpected = true;
                } else if(!xFound && (_currentChar == 'e' || _currentChar == 'E')) {
                    if(eFound) throw LexerException("A number cannot contain more than one exponential part");
                    if(bFound) throw LexerException("A binary number cannot contain an exponential part");
                    if(text == "0") oFound = false;
                    if(oFound) throw LexerException("An octal number cannot contain an exponential part");
                    eFound = true;
                    numberExpected = true;
                } else if(_currentChar == '+' || _currentChar == '-') {
                    if(xFound || !text.endsWith('e', Qt::CaseInsensitive)) break;
                } else if(_currentChar == 'x' || _currentChar == 'X') {
                    if(text != "0") throw LexerException("Unexpected 'x' found in a number (hint: hexadecimal numbers start with '0x')");
                    xFound = true;
                    oFound = false;
                    numberExpected = true;
                } else if(xFound && QString("abcdef").contains(_currentChar, Qt::CaseInsensitive)) {
                    numberExpected = false;
                } else if(_currentChar == 'b' || _currentChar == 'B') {
                    if(text != "0") throw LexerException("Unexpected 'b' found in a number (hint: binary numbers start with '0b')");
                    bFound = true;
                    oFound = false;
                    numberExpected = true;
                } else if(isName()) {
                    throw LexerException(QString("Unexpected character found in a number: '%1'").arg(_currentChar));
                } else break;
                text.append(_currentChar);
                consume();
            }
            if(numberExpected) throw LexerException(QString("Unexpected character found in a number: '%1'").arg(_currentChar));
            return(Token(Number, text));
        }

        const Token textToken() { // TODO: Add support for escape characters
            QString text;
            consume();
            while(_currentChar != '"') {
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found while reading a text literal");
                text.append(_currentChar);
                consume();
            };
            consume();
            return(Token(Text, text));
        }

        void test() {
            setSource("i = 123");
            while(true) {
                Token token = nextToken();
                p(QString("%1: '%2'").arg(tokenTypesName[token.type]).arg(token.text));
                if(token.type == Eof) break;
            }
        }

    private:
        QString _source;
        int _cursor;
        QChar _currentChar;
    };
}

#endif // LEXER_H
