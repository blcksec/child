#ifndef CHILD_LANGUAGE_SOURCE_CODE_H
#define CHILD_LANGUAGE_SOURCE_CODE_H

#include "node/object/text.h"
#include "node/object/block.h"
#include "node/object/language/parser.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_SOURCE_CODE(ARGS...) \
    new Language::SourceCode(context()->child("Object", "Language", "SourceCode"), ##ARGS)

    class SourceCode : public Object {
        CHILD_DECLARE(SourceCode, Object, Language);
    public:
        explicit SourceCode(Node *origin, const QString &url = "",
                   const QString &txt = "", Block *block = NULL);

        virtual ~SourceCode() {
            setBlock(NULL);
        }

        CHILD_FORK_METHOD(SourceCode, url(), text(), CHILD_FORK_IF_NOT_NULL(block()));

        const QString &url() const { return _url; }
        void setUrl(const QString &url) { _url = QFileInfo(url).absoluteFilePath(); }

        const QString &text() const { return _text; }
        void setText(const QString &text) { _text = text; }

        Block *block() const { return _block; }

        void setBlock(Block *block) {
            if(block != _block) {
                if(_block) removeAnonymousChild(_block);
                _block = block;
                if(block) addAnonymousChild(block);
            }
        }

        void load(const QString &newUrl = "");
        void parse(const QString &newText = "");

        virtual Node *run(Node *receiver = context()) {
            #ifdef CHILD_CATCH_EXCEPTIONS
            try {
            #endif
                return block() && block()->bodySection() ? block()->bodySection()->run(receiver) : NULL;
            #ifdef CHILD_CATCH_EXCEPTIONS
            } catch(ExceptionPointer e) {
                e->file = _url;
                throw;
            }
            #endif
        }

        virtual QString toString(bool debug = false, short level = 0) const;
    private:
        QString _url;
        QString _text;
        Block *_block;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_SOURCE_CODE_H
