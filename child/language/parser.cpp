#include "child/language/parser.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINITION(Parser, Object);
}

CHILD_END

//        enum Rule {
//            ListRule
//        };

//        const Token &token(const int i = 0) {
//            loadToken(i);
//            return _lookahead.at(_position + i);
//        }

//        const Token::Type tokenType(const int i = 0) {
//            return token(i).type;
//        }

//        const QString tokenTypeName(const int i = 0) {
//            return token(i).typeName();
//        }

//        const QString tokenText(const int i = 0) {
//            return token(i).text();
//        }

//        void loadToken(const int i) {
//            int n = (_position + i) - (_lookahead.size() - 1);
//            if(n > 0) // out of tokens?
//                for(int i = 1; i <= n; i++)
//                    _lookahead.append(_lexer.nextToken());
//        }

//        void consume() {
//            _position++;
//            if(_position == _lookahead.size() && !isSpeculating()) {
//                _position = 0;
//                _lookahead.clear();
//                clearRulesMemos();
//            }
//            loadToken(0);
//        }

//        void seek(const int position) {
//            _position = position;
//        }

//        void openToken() {
//            _openedTokens.push(token());
//        }

//        void closeToken() {
//            _openedTokens.pop();
//        }

//        void pushPosition() {
//            _positions.push(_position);
//        }

//        void popPosition() {
//            seek(_positions.pop());
//        }

//        const bool isSpeculating() const {
//            return !_positions.isEmpty();
//        }

//        void memoize(const Rule rule, const int startPosition, const bool failed) {
//            int stopPosition = failed ? -1 : _position;
//            _rulesMemos[rule].insert(startPosition, stopPosition);
//        }

//        const bool isMemoized(const Rule rule) {
//            if(!_rulesMemos.contains(rule)) return false;
//            _MemoHash &memos = _rulesMemos[rule];
//            if(!memos.contains(_position)) return false;
//            int stopPosition = memos[_position];
//            if(stopPosition == -1) throw ParserException();
//            seek(stopPosition);
//            return true;
//        }

//        void clearRulesMemos() {
//            _rulesMemos.clear();
//        }

//    private:
//        QList<Token> _lookahead;
//        int _position;
//        QStack<int> _positions;
//        typedef QHash<int, int> _MemoHash;
//        QHash<Rule, _MemoHash> _rulesMemos;

//        void matchElement() {
//            if(is(Token::Name))
//                match(Token::Name);
//            else if(is(Token::LeftBracket))
//                matchList();
//            else
//                throw ParserException("expecting Name or List, found " + tokenTypeName());
//        }

//        void matchElements() {
//            matchElement();
//            while(is(Token::Comma)) {
//                match(Token::Comma);
//                matchElement();
//            }
//        }

//        void matchList() {
//            bool failed = false;
//            QString exceptionMessage;
//            int startPosition = _position;
//            if(isSpeculating() && isMemoized(ListRule)) return;
//            try { _matchList(); }
//            catch(ParserException e) { failed = true; exceptionMessage = e.message; }
//            if(isSpeculating()) memoize(ListRule, startPosition, failed);
//            if(failed) throw ParserException(exceptionMessage);
//        }

//        void _matchList() {
//            p("parse list at: " + QString::number(_position));
//            match(Token::LeftBracket);
//            matchElements();
//            match(Token::RightBracket);
//        }

//        void matchAssignment() {
//            matchList();
//            match(Token::Operator, "=");
//            matchList();
//        }

//        void matchStatement() {
//            if(speculateList()) {
//                matchList(); match(Token::Eof);
//            } else if(speculateAssignment()) {
//                matchAssignment(); match(Token::Eof);
//            } else
//                throw ParserException("expecting Statement, found " + tokenTypeName());
//        }

//        const bool speculateList() {
//            bool success = true;
//            pushPosition();
//            try { matchList(); match(Token::Eof); }
//            catch(ParserException e) { success = false; }
//            popPosition();
//            return success;
//        }

//        const bool speculateAssignment() {
//            bool success = true;
//            pushPosition();
//            try { matchAssignment(); match(Token::Eof); }
//            catch(ParserException e) { success = false; }
//            popPosition();
//            return success;
//        }
