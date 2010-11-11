#ifndef LEXER_H
#define LEXER_H

#include "child/node.h"
#include "child/token.h"
#include "child/operator.h"

namespace Child {
    class Lexer {
    public:
        static const QString allowedCharsInNames;

        Lexer(const QString &src = "") {
            setSource(src);
            initOperators();
        }

        void setSource(const QString &source) {
            _source = source;
            _cursor = -1;
            consume();
        }

        const QString &source() const { return(_source); }

        void initOperators() {
            addOperator("=", Operator::Binary);
            addOperator("//", Operator::LineComment);
            addOperator("/*", Operator::BlockComment);
        }

        void addOperator(const QString &text, Operator::Type type) {
            operators.insert(text, Operator(text, type));
        }

        const Token nextToken() {
            while(!_currentChar.isNull()) {
                if(_currentChar.isSpace()) {
                    consumeSpaces();
                } else if(isName()) {
                    return(nameToken());
                } else if(_currentChar.isNumber()) {
                    return(numberToken());
                } else if(_currentChar == '"') {
                    return(textToken());
                } else if(isOperator()) {
                    return(operatorToken());
                } else {
                    throw LexerException(QString("Invalid character: '%1'").arg(_currentChar));
                }
            }
            return(Token(Token::Eof, "<EOF>"));
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
            return(Token(Token::Name, text));
        }

        bool isOperator() {
            QHashIterator<QString, Operator> i(operators);
            while (i.hasNext()) {
                i.next();
                if(i.key().startsWith(_currentChar)) return(true);
            }
            return(false);
        }

        const Token operatorToken() { // TODO: Add support for operators with more than one character
            QString text;
            text.append(_currentChar);
            consume();
            return(Token(Token::Operator, text));
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
                } else if(_currentChar == '.') {
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

        const Token textToken() {
            QString text;
            consume(); // opening double quote
            bool escapeSequence = false;
            bool xFound;
            QString number;
            while(_currentChar != '"' || escapeSequence) {
                if(_currentChar.isNull()) throw LexerException("Unexpected EOF found while reading a text literal");
                if(escapeSequence) {
                    if(_currentChar == 'x' && number.isEmpty() && !xFound) {
                        xFound = true;
                    } else if(_currentChar >= '0' && _currentChar <= '7') {
                        number.append(_currentChar);
                    } else if(xFound && QString("89abcdef").contains(_currentChar, Qt::CaseInsensitive)) {
                        number.append(_currentChar);
                    } else if(!number.isEmpty() || xFound) {
                        if(number.isEmpty()) throw LexerException("Invalid escape sequence syntax");
                        bool ok;
                        int code = xFound ? number.toInt(&ok, 16) : number.toInt(&ok, 8);
                        if(!ok) throw LexerException("Invalid number in escape sequence");
                        text.append(QChar(code));
                        escapeSequence = false;
                        continue; // Don't consume the current character
                    } else {
                        switch(_currentChar.toAscii()) {
                        case 't':
                            text.append('\t');
                            break;
                        case 'n':
                            text.append('\n');
                            break;
                        case 'r':
                            text.append('\r');
                            break;
                        case '"':
                            text.append('"');
                            break;
                        case '\\':
                            text.append('\\');
                            break;
                        default:
                            throw LexerException(QString("Unknown escape sequence: '\\%1'").arg(_currentChar));
                        }
                        escapeSequence = false;
                    }
                } else if(_currentChar == '\\') {
                    escapeSequence = true;
                    number = "";
                    xFound = false;
                } else {
                    text.append(_currentChar);
                }
                consume();
            };
            consume(); // closing double quote
            return(Token(Token::Text, text));
        }

        void test() {
            setSource("i = 2");
            while(true) {
                Token token = nextToken();
                p(token.toString());
                if(token.type == Token::Eof) break;
            }
        }

    private:
        QString _source;
        int _cursor;
        QChar _currentChar;
        QHash<QString, Operator> operators;
    };
}

#endif // LEXER_H
