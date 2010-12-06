#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "child/element.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Text, Element);

#define CHILD_TEXT(ARGS...) TextPtr(new Text(Node::findInContext("Object")->child("Text"), ##ARGS))

class Text : public GenericElement<TextPtr, QString> {
    CHILD_DECLARATION(Text, Element);
public:
    Text(const NodePtr &origin, const QString &value = "") : GenericElement<TextPtr, QString>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Text", root()); }
    virtual NodePtr fork() const { return new Text(this, value()); }

    TextPtr upcase() { return CHILD_TEXT(value().toUpper()); }

    virtual Comparison compare(const Node &other) const {
        if(this == &other) return Equal;
        TextPtr otherText(other, true);
        if(!otherText) return Different;
        int result = value().compare(otherText->value());
        if(result > 0) return Greater;
        else if(result < 0) return Smaller;
        else return Equal;
    }

    static QString unescapeSequence(const QString &source);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual const QString inspect() const { return QString("\"%1\"").arg(value()); }
};

CHILD_PTR_DEFINITION(Text, Element);

CHILD_END

#endif // CHILD_TEXT_H
