#include "child/language/lexer.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Lexer, Object);

    void Lexer::rewind() {
        _operatorTable.clear();
        _previousChar = '\0';
        _currentChar = '\0';
        _nextChar = '\0';
        _position = -1;
        consume();
    }

    TokenPointer Lexer::nextToken() {
        while(true) {
            switch(_currentChar.toAscii()) {
            case '\'': return scanCharacter();
            case '"': return scanText();
            case '(': return scan(Token::LeftParenthesis);
            case ')': return scan(Token::RightParenthesis);
            case '[': return scan(Token::LeftBracket);
            case ']': return scan(Token::RightBracket);
            case '{': return scan(Token::LeftBrace);
            case '}': return scan(Token::RightBrace);
            case ';': return scan(Token::Semicolon);
            default:
                if(isEof()) return scan(Token::Eof);
                else if(isLineComment()) consumeLineComment();
                else if(isBlockComment()) consumeBlockComment();
                else if(isNewline()) return scanNewline();
                else if(isSpace()) consumeSpaces();
                else if(isName()) return scanName();
                else if(isNumber()) return scanNumber();
                else if(isOperator()) return scanOperator();
                else throw lexerException(QString("invalid character: '%1'").arg(_currentChar));
            }
        }
    }

    void Lexer::consume() {
        _previousChar = _currentChar;
        _position++;
        if(_position < source().length()) {
            _currentChar = source().at(_position);
            if(_position + 1 < source().length())
                _nextChar = source().at(_position + 1);
            else
                _nextChar = QChar::Null;
        } else {
            _currentChar = QChar::Null;
            _nextChar = QChar::Null;
        }
    }

    void Lexer::consumeLineComment() {
        consume(); // First slash
        consume(); // Second slash
        while(!(isNewline() || isEof())) consume();
    }

    void Lexer::consumeBlockComment() {
        consume(); // Slash
        consume(); // *
        while(!(_currentChar == '*' && _nextChar == '/')) {
            if(isEof()) throw lexerException("unexpected EOF found before the end of a comment");
            consume();
        }
        consume(); // *
        consume(); // Slash
    }

    TokenPointer Lexer::scanName() {
        startToken();
        consume();
        while(_currentChar.isLetterOrNumber() || _currentChar == '_' || _currentChar == '!' || _currentChar == '?')
            consume();
        const QStringRef text(tokenTextRef());
        if(text == "true" || text == "false") return finishToken(Token::Boolean);
        return finishToken(Token::Name);
    }

    TokenPointer Lexer::scanOperator() {
        startToken();
        QString text(_currentChar);
        do {
            consume();
            text.append(_currentChar);
        } while(operatorTable()->has(text));
        return finishToken(Token::Operator);
    }

    TokenPointer Lexer::scanNumber() {
        startToken();
        consume();
        short base = 10;
        bool decimalPointFound = false;
        bool eFound = false;
        bool oneMoreDigitExpected = false;
        if(_previousChar == '0') {
            if(_currentChar == 'x' || _currentChar == 'X') {
                base = 16;
                consume();
                oneMoreDigitExpected = true;
            } else if(_currentChar.isNumber()) {
                base = 8;
                oneMoreDigitExpected = true;
            } else if(_currentChar == 'b' || _currentChar == 'B') {
                base = 2;
                consume();
                oneMoreDigitExpected = true;
            }
        }
        while(true) {
            if(_currentChar.isNumber()) {
                if(base == 2 && !(_currentChar == '0' || _currentChar == '1'))
                    throw lexerException("a binary number can only contain 0 or 1");
                if(base == 8 && !QString("01234567").contains(_currentChar))
                    throw lexerException("an octal number can only contain digits from 0 to 7");
                oneMoreDigitExpected = false;
            } else if(base == 16 && QString("abcdef").contains(_currentChar, Qt::CaseInsensitive)) {
                oneMoreDigitExpected = false;
            } else if(_currentChar == '.' && _nextChar.isNumber()) {
                if(decimalPointFound) throw lexerException("too many decimal points in a number");
                if(eFound) throw lexerException("the exponential part of a number cannot contain a decimal point");
                if(base == 16) throw lexerException("an hexadecimal number cannot contain a decimal point");
                if(base == 8) throw lexerException("an octal number cannot contain a decimal point");
                if(base == 2) throw lexerException("a binary number cannot contain a decimal point");
                decimalPointFound = true;
                oneMoreDigitExpected = true;
            } else if(base != 16 && (_currentChar == 'e' || _currentChar == 'E')) {
                if(eFound) throw lexerException("a number cannot contain more than one exponential part");
                if(base == 8) throw lexerException("an octal number cannot contain an exponential part");
                if(base == 2) throw lexerException("a binary number cannot contain an exponential part");
                eFound = true;
                oneMoreDigitExpected = true;
            } else if((_currentChar == '+' || _currentChar == '-') && base == 10 && (_previousChar == 'e' || _previousChar == 'E')) {
                // All is good in the hood!
            } else if(isName()) {
                throw lexerException(QString("unexpected character found in a number: '%1'").arg(_currentChar));
            } else break;
            consume();
        }
        if(oneMoreDigitExpected) throw lexerException(QString("unexpected character found in a number: '%1'").arg(_currentChar));
        return finishToken(Token::Number);
    }

    TokenPointer Lexer::scanCharacter() {
        startToken();
        consume(); // left single quote
        if(isEof()) throw lexerException("unexpected EOF found in a character literal");
        if(_currentChar != '\'') {
            if(_currentChar == '\\') consumeEscapeSequence(); else consume();
            if(isEof()) throw lexerException("unexpected EOF found in a character literal");
            if(_currentChar != '\'') throw lexerException("a character literal can't have more than one character");
        }
        consume(); // right single quote
        return finishToken(Token::Character);
    }

    TokenPointer Lexer::scanText() {
        startToken();
        consume(); // left double quote
        while(_currentChar != '"') {
            if(isEof()) throw lexerException("unexpected EOF found in a text literal");
            if(_currentChar == '\\') consumeEscapeSequence(); else consume();
        };
        consume(); // right double quote
        return finishToken(Token::Text);
    }

    void Lexer::consumeEscapeSequence() {
        consume(); // anti-slash
        if(isEof()) throw lexerException("unexpected EOF found in an escape sequence");
        if(QString("trn\"'\\").contains(_currentChar))
            consume();
        else if(QString("01234567xu").contains(_currentChar, Qt::CaseInsensitive))
            consumeEscapeSequenceNumber();
        else throw lexerException(QString("unknown escape sequence: '\\%1'").arg(_currentChar));
    }

    void Lexer::consumeEscapeSequenceNumber() {
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
            if(isEof()) throw lexerException("unexpected EOF found in an escape sequence number");
            if(!allowedChars.contains(_currentChar, Qt::CaseInsensitive)) break;
            number.append(_currentChar);
            consume();
        }
        if(number.isEmpty()) throw lexerException("invalid escape sequence number");
        bool ok;
        ushort code = type == 'o' ? number.toUShort(&ok, 8) : number.toUShort(&ok, 16);
        if(!ok) throw lexerException("invalid number in escape sequence");
        if(type != 'u' && code > 0xFF) throw lexerException("invalid number in escape sequence");
    }

    LexerExceptionPointer Lexer::lexerException(QString message) const {
        int column, line;
        computeColumnAndLineForPosition(source(), _position, column, line);
        QString text = extractLine(source(), line);
        if(!text.isEmpty()) {
            QString cursor = QString(" ").repeated(column - 1).append("^");
            message += "\n" + text + "\n" + cursor;
        }
        return new LexerException(context()->child("LexerException"), message, resourceName(), line);
    }

    const QString Lexer::toString(bool debug, short level) const {
        QString str;
        const_cast<Lexer *>(this)->rewind();
        while(true) {
            TokenPointer token = const_cast<Lexer *>(this)->nextToken();
            if(token->type == Token::Eof) break;
            if(!str.isEmpty()) str += ", ";
            str += token->toString(debug, level);
        }
        return "[" + str + "]";
    }
}

CHILD_END
