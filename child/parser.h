#ifndef PARSER_H
#define PARSER_H

#include "child/node.h"
#include "child/lexer.h"

namespace Child {
    class Parser {
    public:
        Parser() : _position(0) {}

        const Token &token(const int i = 0) {
            loadToken(i);
            return(_lookahead.at(_position + i));
        }

        const Token::Type tokenType(const int i = 0) {
            return(token(i).type);
        }

        const QString tokenTypeName(const int i = 0) {
            return(token(i).typeName());
        }

        const QString tokenText(const int i = 0) {
            return(token(i).text());
        }

        void loadToken(const int i) {
            int n = (_position + i) - (_lookahead.size() - 1);
            if(n > 0) // out of tokens?
                for(int i = 1; i <= n; i++)
                    _lookahead.append(_lexer.nextToken());
        }

        void consume() {
            _position++;
            if(_position == _lookahead.size() && !isSpeculating()) {
                _position = 0;
                _lookahead.clear();
                clearMemos();
            }
            loadToken(0);
        }

        void seek(const int position) {
            _position = position;
        }

        void pushPosition() {
            _positions.push(_position);
        }

        void popPosition() {
            seek(_positions.pop());
        }

        const bool isSpeculating() {
            return(!_positions.isEmpty());
        }

        void memoize(QHash<int, int> &memo, const int startPosition, const bool failed) {
            int stopPosition = failed ? -1 : _position;
            memo.insert(startPosition, stopPosition);
        }

        const bool isMemoized(const QHash<int, int> &memo) {
            if(!memo.contains(_position)) return(false);
            int stopPosition = memo[_position];
            if(stopPosition == -1) throw(ParserException());
            seek(stopPosition);
            return(true);
        }

        void clearMemos() {
            _listMemo.clear();
        }

        const bool is(const Token::Type type, const QString &text = QString()) {
            if(text.isNull())
                return(tokenType() == type);
            else
                return(tokenType() == type && tokenText() == text);
        }

        void match(const Token::Type type, const QString &text = QString()) {
            if(is(type, text)) {
//                if(!isSpeculating()) p(token().toString());
                consume();
            } else {
                const QString optionalText = text.isNull() ? "" : "'" + text + "' ";
                throw(ParserException("expecting " + optionalText + Token::typeName(type) + ", found " + tokenTypeName()));
            }
        }

        void matchElement() {
            if(is(Token::Name))
                match(Token::Name);
            else if(is(Token::LeftBracket))
                matchList();
            else
                throw(ParserException("expecting Name or List, found " + tokenTypeName()));
        }

        void matchElements() {
            matchElement();
            while(is(Token::Comma)) {
                match(Token::Comma);
                matchElement();
            }
        }

        void matchList() {
            bool failed = false;
            QString exceptionMessage;
            int startPosition = _position;
            if(isSpeculating() && isMemoized(_listMemo)) return;
            try { _matchList(); }
            catch(ParserException e) { failed = true; exceptionMessage = e.message; }
            if(isSpeculating()) memoize(_listMemo, startPosition, failed);
            if(failed) throw(ParserException(exceptionMessage));
        }

        void _matchList() {
            p("parse list at: " + QString::number(_position));
            match(Token::LeftBracket);
            matchElements();
            match(Token::RightBracket);
        }

        void matchAssignment() {
            matchList();
            match(Token::Operator, "=");
            matchList();
        }

        void matchStatement() {
            if(speculateList()) {
                matchList(); match(Token::Eof);
            } else if(speculateAssignment()) {
                matchAssignment(); match(Token::Eof);
            } else
                throw(ParserException("expecting Statement, found " + tokenTypeName()));
        }

        const bool speculateList() {
            bool success = true;
            pushPosition();
            try { matchList(); match(Token::Eof); }
            catch(ParserException e) { success = false; }
            popPosition();
            return(success);
        }

        const bool speculateAssignment() {
            bool success = true;
            pushPosition();
            try { matchAssignment(); match(Token::Eof); }
            catch(ParserException e) { success = false; }
            popPosition();
            return(success);
        }

        void test() {
            _lexer = Lexer("[a] = [b]");
            matchStatement();
        }

    private:
        Lexer _lexer;
        QStack<int> _positions;
        QList<Token> _lookahead;
        int _position;
        QHash<int, int> _listMemo;
        typedef void (*_FunctionPtr)();
        typedef QHash<int, int> _MemoHash;
        QHash<_FunctionPtr, _MemoHash> _essai;
    };
}

#endif // PARSER_H

