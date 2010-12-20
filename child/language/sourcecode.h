#ifndef CHILD_LANGUAGE_SOURCE_CODE_H
#define CHILD_LANGUAGE_SOURCE_CODE_H

#include "child/text.h"
#include "child/dictionary.h"
#include "child/block.h"
#include "child/language/parser.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(SourceCode, Object);

    #define CHILD_SOURCE_CODE(ARGS...) \
    Language::SourceCodePointer(new Language::SourceCode(Node::context()->child("Object", "Language", "SourceCode"), ##ARGS))

    class SourceCode : public Object {
        CHILD_DECLARE(SourceCode, Object);
    public:
        SourceCode(const Pointer &origin, const QString &url = "",
                   const QString &txt = "", const BlockPointer &block = NULL) :
            Object(origin), _url(url), _text(txt), _block(block) {
            if(!url.isEmpty() && txt.isEmpty()) load();
            if(!text().isEmpty() && block.isNull()) parse();
        }

        static void initRoot() { Language::root()->addChild("SourceCode", root()); }

        virtual Pointer fork() const {
            return new SourceCode(this, url(), text(), block() ? block()->fork() : block());
        }

        const QString &url() const { return _url; }
        void setUrl(const QString &url) { _url = QFileInfo(url).absoluteFilePath(); }

        const QString &text() const { return _text; }
        void setText(const QString &text) { _text = text; }

        BlockPointer block() const { return _block; }
        void setBlock(const BlockPointer &block) { _block = block; }

        void load(const QString &newUrl = "") {
            if(!newUrl.isEmpty()) setUrl(newUrl);
            if(url().isEmpty()) CHILD_THROW(ArgumentException, "URL is empty");
            setText(readTextFile(url()));
        }

        void parse(const QString &newText = "") {
            if(!newText.isEmpty()) setText(newText);
            ParserPointer parser = context()->child("parser");
            setBlock(parser->parse(text(), url()));
        }

        virtual Pointer run(const Pointer &receiver = context()) {
            #ifdef CHILD_CATCH_EXCEPTIONS
            try {
            #endif
                return block() ? block()->run(receiver) : Pointer::null();
            #ifdef CHILD_CATCH_EXCEPTIONS
            } catch(ExceptionPointer e) {
                e->file = _url;
                throw;
            }
            #endif
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            QString str;
            if(!url().isEmpty()) str += QString("/* %1 */\n").arg(url());
            if(block()) str += block()->toString(debug, level);
            return str;
        }
    private:
        QString _url;
        QString _text;
        BlockPointer _block;
    };

    CHILD_POINTER_DEFINE(SourceCode, Object);

    // === SourceCodeDictionary ===

    CHILD_POINTER_DECLARE(SourceCodeDictionary, Dictionary);

    #define CHILD_SOURCE_CODE_DICTIONARY(ARGS...) \
    Language::SourceCodeDictionaryPointer(new Language::SourceCodeDictionary( \
        Node::context()->child("Object", "Language", "SourceCodeDictionary"), ##ARGS))

    class SourceCodeDictionary : public GenericDictionary<SourceCodeDictionaryPointer, Reference, SourceCodePointer> {
        CHILD_DECLARE(SourceCodeDictionary, Dictionary);
    public:
        SourceCodeDictionary(const Pointer &origin) :
            GenericDictionary<SourceCodeDictionaryPointer, Reference, SourceCodePointer>(origin) {}

        static void initRoot() { Language::root()->addChild("SourceCodeDictionary", root()); }
        virtual Pointer fork() const { return SourceCodeDictionaryPointer(new SourceCodeDictionary(this))->initFork(); };
    };

    CHILD_POINTER_DEFINE(SourceCodeDictionary, Dictionary);
}

CHILD_END

#endif // CHILD_LANGUAGE_SOURCE_CODE_H
