#ifndef LEXER_H
#define LEXER_H

#include "child/node.h"
#include "child/token.h"
#include "child/operator.h"

namespace Child {
    class Lexer {
    public:
        Lexer(const QString &src = "") {
            setSource(src);
        }

        void setSource(const QString &source) {
            _source = source;
            rewind();
        }

        const QString &source() const { return(_source); }

        void rewind() {
            _cursor = -1;
            consume();
        }

        const Token nextToken() {
            while(true) {
                switch(_currentChar.toAscii()) {
                case '\'': return(characterToken());
                case '"': return(textToken());
                case '(': return(Token(Token::LeftParenthesis, consume()));
                case ')': return(Token(Token::RightParenthesis, consume()));
                case '[': return(Token(Token::LeftBracket, consume()));
                case ']': return(Token(Token::RightBracket, consume()));
                case '{': return(Token(Token::LeftBrace, consume()));
                case '}': return(Token(Token::RightBrace, consume()));
                case ',': return(Token(Token::Comma, consume()));
                case ';': return(Token(Token::Semicolon, consume()));
                case '\0': consume(); return(Token(Token::Eof, "<EOF>"));
                default:
                    if(_currentChar == ':' && _nextChar != '=') return(Token(Token::Colon, consume()));
                    else if(_currentChar == '/' && _nextChar == '/') consumeLineComment();
                    else if(_currentChar == '/' && _nextChar == '*') consumeBlockComment();
                    else if(_currentChar.isSpace()) consumeSpaces();
                    else if(isName()) return(nameToken());
                    else if(_currentChar.isNumber()) return(numberToken());
                    else if(isOperator()) return(operatorToken());
                    else throw LexerException(QString("Invalid character: '%1'").arg(_currentChar));
                }
            }
        }

        const QChar consume() {
            QChar previousChar(_currentChar);
            _cursor++;
            if(_cursor < _source.length()) {
                _currentChar = _source.at(_cursor);
                if(_cursor + 1 < _source.length())
                    _nextChar = _source.at(_cursor + 1);
                else
                    _nextChar = QChar::Null;
            } else {
                _currentChar = QChar::Null;
                _nextChar = QChar::Null;
            }
            return(previousChar);
        }

        void consumeSpaces() {
            do { consume(); } while(_currentChar.isSpace());
        }

        void consumeLineComment() {
            consume(); // First slash
            consume(); // Second slash
            while(_currentChar != '\r' && _currentChar != '\n' && !_currentChar.isNull()) consume();
        }

        void consumeBlockComment() {
            consume(); // Slash
            consume(); // *
            while(!(_currentChar == '*' && _nextChar == '/')) {
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found before the end of a comment");
                consume();
            }
            consume(); // *
            consume(); // Slash
        }

        bool isName() {
            return(_currentChar.isLetter() || _currentChar == '_');
        }

        const Token nameToken() {
            QString text(_currentChar);
            consume();
            while(_currentChar.isLetterOrNumber() || _currentChar == '_' || _currentChar == '!' || _currentChar == '?') {
                text.append(_currentChar);
                consume();
            }
            if(text == "true" || text == "false") return(booleanToken(text));
            return(Token(Token::Name, text));
        }

        bool isOperator() {
            return(Operator::allowedChars.contains(_currentChar));
        }

        const Token operatorToken() {
            QString text(_currentChar);
            consume();
            while(isOperator()) {
                text.append(_currentChar);
                consume();
            }
            return(Token(Token::Operator, text));
        }

        const Token booleanToken(const QString &text) {
            return(Token(Token::Boolean, text));
        }

        const Token numberToken() {
            QString text(_currentChar);
            consume();
            bool pointFound = false;
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
                } else if(_currentChar == '.' && !Operator::allowedChars.contains(_nextChar)) {
                    if(pointFound) throw LexerException("Too many decimal points in a number");
                    if(eFound) throw LexerException("The exponential part of a number cannot contain a decimal point");
                    if(xFound) throw LexerException("An hexadecimal number cannot contain a decimal point");
                    if(bFound) throw LexerException("A binary number cannot contain a decimal point");
                    if(text == "0") oFound = false;
                    if(oFound) throw LexerException("An octal number cannot contain a decimal point");
                    pointFound = true;
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
            return(Token(Token::Number, text));
        }

        const Token characterToken() {
            QString text;
            consume(); // left single quote
            if(_currentChar.isNull()) throw LexerException("Unexpected EOF found in a character literal");
            if(_currentChar != '\'') {
                if(_currentChar == '\\') {
                    text = escapeSequence();
                } else {
                    text = _currentChar;
                    consume();
                }
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found in a character literal");
                if(_currentChar != '\'') throw LexerException("A character literal can't have more than one character");
            }
            consume(); // right single quote
            return(Token(Token::Character, text));
        }

        const Token textToken() {
            QString text;
            consume(); // left double quote
            while(_currentChar != '"') {
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found in a text literal");
                if(_currentChar == '\\') {
                    text.append(escapeSequence());
                } else {
                    text.append(_currentChar);
                    consume();
                }
            };
            consume(); // right double quote
            return(Token(Token::Text, text));
        }

        QChar escapeSequence() {
            consume(); // anti-slash
            QChar character;
            switch(_currentChar.toAscii()) {
            case 't':
                character = '\t';
                break;
            case 'n':
                character = '\n';
                break;
            case 'r':
                character = '\r';
                break;
            case '"':
                character = '"';
                break;
            case '\'':
                character = '\'';
                break;
            case '\\':
                character = '\\';
                break;
            case '\0':
                throw LexerException("Unexpected EOF found in an escape sequence");
            default:
                if(QString("01234567xu").contains(_currentChar, Qt::CaseInsensitive)) {
                    return(escapeSequenceNumber());
                } else {
                    throw LexerException(QString("Unknown escape sequence: '\\%1'").arg(_currentChar));
                }
            }
            consume();
            return(character);
        }

        QChar escapeSequenceNumber() {
            char type;
            QString allowedChars;
            short maxSize;
            if(_currentChar == 'x' || _currentChar == 'X') {
                type = 'x';
                allowedChars = "0123456789abcdef";
                maxSize = 2;
                consume();
            } else if(_currentChar == 'u' || _currentChar == 'U') {
                type = 'u';
                allowedChars = "0123456789abcdef";
                maxSize = 4;
                consume();
            } else {
                type = 'o';
                allowedChars = "01234567";
                maxSize = 3;
            }
            QString number = "";
            while(number.size() < maxSize) {
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found in an escape sequence number");
                if(!allowedChars.contains(_currentChar, Qt::CaseInsensitive)) break;
                number.append(_currentChar);
                consume();
            }
            if(number.isEmpty()) throw LexerException("Invalid escape sequence number");
            bool ok;
            ushort code = type == 'o' ? number.toUShort(&ok, 8) : number.toUShort(&ok, 16);
            if(!ok) throw LexerException("Invalid number in escape sequence");
            if(type != 'u' && code > 0xFF) throw LexerException("Invalid number in escape sequence");
            return(QChar(code));
        }

        const QString toString() {
            QString result;
            while(true) {
                Token token = nextToken();
                if(token.type == Token::Eof) break;
                if(!result.isEmpty()) result.append(", ");
                result.append(token.toString());
            }
            return("[" + result + "]");
        }

        void test1() {
            setSource("[1..3]");
            p(toString());
        }

        void test2() {
            setSource(readTextFile("../child/examples/lexertest.child"));
            p(toString());
        }

        void test() {
            setSource(readTextFile("../child/examples/lexertest.child"));
            for(int i = 0; i < 1000; i++) {
                while(nextToken().type != Token::Eof) {}
                rewind();
            }
        }

    private:
        QString _source;
        int _cursor;
        QChar _currentChar;
        QChar _nextChar;
        QHash<QString, Operator> operators;
    };
}

#endif // LEXER_H
