#include "node/language/sourcecode.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(SourceCode, Object);

    SourceCode::SourceCode(Node *origin, const QString &url,
               const QString &txt, Block *block) :
        Object(origin), _url(url), _text(txt), _block(block) {
        if(!url.isEmpty() && txt.isEmpty()) load();
    }

    void SourceCode::initRoot() {
        Language::root()->addChild("SourceCode", this);
    }

    void SourceCode::load(const QString &newUrl) {
        if(!newUrl.isEmpty()) setUrl(newUrl);
        if(url().isEmpty()) CHILD_THROW(ArgumentException, "URL is empty");
        setText(readTextFile(url()));
    }

    void SourceCode::parse(const QString &newText) {
        if(!newText.isEmpty()) setText(newText);
        Parser *parser = Parser::cast(child("parser")); // search a Parser in parents
        setBlock(parser->parse(text(), url()));
    }

    QString SourceCode::toString(bool debug, short level) const {
        QString str;
        if(!url().isEmpty()) str += QString("/* %1 */\n").arg(url());
        if(block()) str += block()->toString(debug, level);
        return str;
    }
}

CHILD_END
