#ifndef CHILD_SOURCECODE_H
#define CHILD_SOURCECODE_H

#include "child/object.h"
#include "child/parser.h"
#include "child/block.h"

#define CHILD_SOURCECODE(EXPRESSION) static_cast<SourceCode *>(EXPRESSION)

namespace Child {
    class SourceCode : public Object {
    public:
        static SourceCode *root();
        static SourceCode *fork(Node *world) { return(CHILD_SOURCECODE(world->child("SourceCode"))->fork()); }

        SourceCode() : _block(NULL) {}

        virtual ~SourceCode() {
            delete _block;
        }

        virtual SourceCode *fork() { return(_fork(this)); }

        Block *block() const { return(_block); }
        void setBlock(Block *block) {
            if(block == _block) return;
            delete _block;
            _block = block;
        }

        const QString &text() const { return(_text); }
        void setText(const QString &text) { _text = text; }

        const QString &url() const { return(_url); }
        void setUrl(const QString &url) { _url = QFileInfo(url).absoluteFilePath(); }

        void load(const QString &url) {
            setText(readTextFile(url));
            setUrl(url);
        }

        void parse(Parser *parser) {
            setBlock(parser->parse(text(), QFileInfo(url()).fileName()));
        }

        virtual const QString inspect() const {
            QString str;
            if(!url().isEmpty()) str.append(QString("/* %1 */\n").arg(url()));
            if(block()) str.append(block()->inspect());
            return(str);
        }
    private:
        static SourceCode *_root;
        Block *_block;
        QString _text;
        QString _url;
    };
}

#endif // CHILD_SOURCECODE_H
