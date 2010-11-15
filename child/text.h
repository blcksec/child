#ifndef STRING_H
#define STRING_H

#include "child/nativemethod.h"
#include "child/object.h"

#define CHILD_TEXT(EXPRESSION) static_cast<Text *>(EXPRESSION)

namespace Child {
    class Text : public Object {
    public:
        static Text *root();

        virtual Text *fork() { return(_fork(this)->setValue(_value)); }
        virtual Text *fork(const QString &value) { return(_fork(this)->setValue(value)); }

        const QString value() const { return(_value); }
        Text *setValue(const QString &value) { _value = value; return(this); }

        Node *upcase() { return(CHILD_TEXT(child("Text"))->fork(value().toUpper())); }

//        QChar escapeSequence() { // Code saved from the Lexer class. Can be more useful here!
//            consume(); // anti-slash
//            if(_currentChar.isNull()) throw(LexerException("unexpected EOF found in an escape sequence"));
//            QChar character;
//            switch(_currentChar.toAscii()) {
//            case 't':
//                character = '\t';
//                break;
//            case 'n':
//                character = '\n';
//                break;
//            case 'r':
//                character = '\r';
//                break;
//            case '"':
//                character = '"';
//                break;
//            case '\'':
//                character = '\'';
//                break;
//            case '\\':
//                character = '\\';
//                break;
//            default:
//                if(QString("01234567xu").contains(_currentChar, Qt::CaseInsensitive))
//                    return(escapeSequenceNumber());
//                else
//                    throw(LexerException(QString("unknown escape sequence: '\\%1'").arg(_currentChar)));
//            }
//            consume();
//            return(character);
//        }

//        QChar escapeSequenceNumber() { // Code saved from for the Lexer class. Can be more useful here!
//            char type;
//            QString allowedChars;
//            short maxSize;
//            if(_currentChar == 'x' || _currentChar == 'X') {
//                type = 'x';
//                allowedChars = "0123456789abcdef";
//                maxSize = 2;
//                consume();
//            } else if(_currentChar == 'u' || _currentChar == 'U') {
//                type = 'u';
//                allowedChars = "0123456789abcdef";
//                maxSize = 4;
//                consume();
//            } else {
//                type = 'o';
//                allowedChars = "01234567";
//                maxSize = 3;
//            }
//            QString number = "";
//            while(number.size() < maxSize) {
//                if(_currentChar.isNull()) throw(LexerException("unexpected EOF found in an escape sequence number"));
//                if(!allowedChars.contains(_currentChar, Qt::CaseInsensitive)) break;
//                number.append(_currentChar);
//                consume();
//            }
//            if(number.isEmpty()) throw(LexerException("invalid escape sequence number"));
//            bool ok;
//            ushort code = type == 'o' ? number.toUShort(&ok, 8) : number.toUShort(&ok, 16);
//            if(!ok) throw(LexerException("invalid number in escape sequence"));
//            if(type != 'u' && code > 0xFF) throw(LexerException("invalid number in escape sequence"));
//            return(QChar(code));
//        }
    private:
        static Text *_root;
        QString _value;
    };
}

#endif // STRING_H
