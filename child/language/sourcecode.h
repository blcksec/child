#ifndef CHILD_SOURCECODE_H
#define CHILD_SOURCECODE_H

#include <QtCore/QFileInfo>

#include "child/language/block.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(SourceCode, Object);

    #define CHILD_SOURCE_CODE(ARGS...) \
    SourceCodePtr(new SourceCode(Node::findInContext("Object")->child("Language")->child("SourceCode"), ##ARGS))

    class SourceCode : public Object {
        CHILD_DECLARATION(SourceCode, Object);
    public:
        SourceCode(const NodePtr &origin, const QString &url = "",
                   const QString &text = "", const BlockPtr &block = NULL) :
            Object(origin), _url(url), _text(text), _block(block) {
            if(!url.isEmpty() && text.isEmpty()) load();
            if(!text.isEmpty() && block.isNull()) parse();
        }

        static void initRoot() { Language::root()->addChild("SourceCode", root()); }

        virtual NodePtr fork() const {
            return new SourceCode(this, url(), text(), block() ? block()->fork() : block());
        }

        const QString &url() const { return _url; }
        void setUrl(const QString &url) { _url = QFileInfo(url).absoluteFilePath(); }

        const QString &text() const { return _text; }
        void setText(const QString &text) { _text = text; }

        BlockPtr block() const { return _block; }
        void setBlock(const BlockPtr &block) { _block = block; }

        void load(const QString &newUrl = "") {
            if(!newUrl.isEmpty()) setUrl(newUrl);
            if(url().isEmpty()) CHILD_THROW(ArgumentException, "URL is empty");
            setText(readTextFile(url()));
        }

        void parse(const QString &newText = "") {
            if(!newText.isEmpty()) setText(newText);
            #pragma unused(newText)
//            setBlock(parser->parse(text(), QFileInfo(url()).fileName()));
        }

        virtual const QString toString(bool debug = false) const {
            QString str;
            if(!url().isEmpty()) str += QString("/* %1 */\n").arg(url());
            if(block()) str += block()->toString(debug);
            return str;
        }
    private:
        QString _url;
        QString _text;
        BlockPtr _block;
    };

    CHILD_PTR_DEFINITION(SourceCode, Object);

    // === SourceCodeList ===

    CHILD_PTR_DECLARATION(SourceCodeList, List);

    #define CHILD_SOURCE_CODE_LIST(ARGS...) \
    SourceCodeListPtr(new SourceCodeList(Node::findInContext("Object")->child("Language")->child("SourceCodeList"), ##ARGS))

    class SourceCodeList : public GenericList<SourceCodeListPtr, SourceCodePtr> {
        CHILD_DECLARATION(SourceCodeList, List);
    public:
        SourceCodeList(const NodePtr &origin) : GenericList<SourceCodeListPtr, SourceCodePtr>(origin) {}

        static void initRoot() { Language::root()->addChild("SourceCodeList", root()); }
        virtual NodePtr fork() const { CHILD_TODO; return new SourceCodeList(this); }
    };

    CHILD_PTR_DEFINITION(SourceCodeList, List);
}

CHILD_END

#endif // CHILD_SOURCECODE_H
