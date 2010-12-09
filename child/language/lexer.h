#ifndef CHILD_LEXER_H
#define CHILD_LEXER_H

#include "child/language/token.h"
#include "child/language/operatortable.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Lexer, Object);

    #define CHILD_LEXER(ARGS...) \
    Language::LexerPtr(new Language::Lexer(Node::context()->child("Object", "Language", "Lexer"), ##ARGS))

    class Lexer : public Object {
        CHILD_DECLARATION(Lexer, Object);
    public:
        Lexer(const NodePtr &origin) : Object(origin) {}
        static void initRoot() { Language::root()->addChild("Lexer", root()); }
        virtual NodePtr fork() const { return new Lexer(this); } // TODO

        OperatorTablePtr operatorTable() const {
            if(!_operatorTable) const_cast<Lexer *>(this)->_operatorTable = context()->child("operatorTable");
            return _operatorTable;
        }

        void setSource(const QString source) { _source = source; rewind(); }
        const QString source() const { return _source; }

        void setResourceName(const QString &name) { _resourceName = name;}
        const QString resourceName() const { return _resourceName; }

        void rewind();
        TokenPtr nextToken();

        void consume();

        void startToken() {
            _tokenPosition = _position;
        }

        TokenPtr finishToken(const Token::Type type) {
            return CHILD_TOKEN(type, tokenTextRef());
        }

        const QStringRef tokenTextRef() const {
            return _source.midRef(_tokenPosition, _position - _tokenPosition);
        }

        TokenPtr scan(const Token::Type type) { // Simple one char tokens
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

        TokenPtr scanNewline() {
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

        TokenPtr scanName();

        bool isOperator() const {
            return operatorTable()->hasOperatorStartingWith(_currentChar);
        }

        TokenPtr scanOperator();

        bool isNumber() const {
            return _currentChar.isNumber();
        }

        TokenPtr scanNumber();

        TokenPtr scanCharacter();
        TokenPtr scanText();

        void consumeEscapeSequence();
        void consumeEscapeSequenceNumber();

        void throwError(QString message) const;

        virtual const QString toString(bool debug = false) const;

//        void test() {
//            setSource("[a, b] = [c, d]");
//            p(toString().toUtf8());
//        }

//        void test() {
//            setSource(readTextFile("../child/examples/lexertest.child"));
//            setFilename("lexertest.child");
//            p(toString().toUtf8());
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
        OperatorTablePtr _operatorTable;
        QString _source;
        QString _resourceName;
        int _position;
        QChar _previousChar;
        QChar _currentChar;
        QChar _nextChar;
        int _tokenPosition;
    };

    CHILD_PTR_DEFINITION(Lexer, Object);
}

CHILD_END

#endif // CHILD_LEXER_H
