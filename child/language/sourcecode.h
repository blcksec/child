#ifndef CHILD_LANGUAGE_SOURCE_CODE_H
#define CHILD_LANGUAGE_SOURCE_CODE_H

#include <QtCore/QFileInfo>

#include "child/text.h"
#include "child/dictionary.h"
#include "child/block.h"
#include "child/language/parser.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(SourceCode, Object);

    #define CHILD_SOURCE_CODE(ARGS...) \
    Language::SourceCodePtr(new Language::SourceCode(Node::context()->child("Object", "Language", "SourceCode"), ##ARGS))

    class SourceCode : public Object {
        CHILD_DECLARATION(SourceCode, Object);
    public:
        SourceCode(const NodePtr &origin, const QString &url = "",
                   const QString &txt = "", const BlockPtr &block = NULL) :
            Object(origin), _url(url), _text(txt), _block(block) {
            if(!url.isEmpty() && txt.isEmpty()) load();
            if(!text().isEmpty() && block.isNull()) parse();
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
            ParserPtr parser = context()->child("parser");
            setBlock(parser->parse(text(), QFileInfo(url()).fileName()));
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

    // === SourceCodeDictionary ===

    CHILD_PTR_DECLARATION(SourceCodeDictionary, Dictionary);

    #define CHILD_SOURCE_CODE_DICTIONARY(ARGS...) \
    Language::SourceCodeDictionaryPtr(new Language::SourceCodeDictionary( \
        Node::context()->child("Object", "Language", "SourceCodeDictionary"), ##ARGS))

    class SourceCodeDictionary : public GenericDictionary<SourceCodeDictionaryPtr, NodeRef, SourceCodePtr> {
        CHILD_DECLARATION(SourceCodeDictionary, Dictionary);
    public:
        SourceCodeDictionary(const NodePtr &origin) :
            GenericDictionary<SourceCodeDictionaryPtr, NodeRef, SourceCodePtr>(origin) {}

        static void initRoot() { Language::root()->addChild("SourceCodeDictionary", root()); }
        virtual NodePtr fork() const { return SourceCodeDictionaryPtr(new SourceCodeDictionary(this))->initFork(); };
    };

    CHILD_PTR_DEFINITION(SourceCodeDictionary, Dictionary);
}

CHILD_END

#endif // CHILD_LANGUAGE_SOURCE_CODE_H
