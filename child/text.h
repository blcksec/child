#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "child/element.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Text, Element);

#define CHILD_TEXT(ARGS...) TextPointer(new Text(Node::context()->child("Object", "Text"), ##ARGS))

class MessagePointer;

class Text : public GenericElement<TextPointer, QString> {
    CHILD_DECLARATION(Text, Element);
public:
    Text(const Pointer &origin, const QString &value = "") : GenericElement<TextPointer, QString>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Text", root());
        root()->addChild("upcase", new NativeMethod(NativeMethod::root(), static_cast<MethodPtr>(&Text::_upcase_)));
    }
    virtual Pointer fork() const { return new Text(this, value()); }

    TextPointer upcase() { return CHILD_TEXT(value().toUpper()); }
    Pointer _upcase_(const MessagePointer &message) {
        Q_UNUSED(message);
        return upcase();
    }

    virtual Comparison compare(const Node &other) const {
        if(this == &other) return Equal;
        TextPointer otherText(other, true);
        if(!otherText) return Different;
        int result = value().compare(otherText->value());
        if(result > 0) return Greater;
        else if(result < 0) return Smaller;
        else return Equal;
    }

    static QString unescapeSequence(const QString &source);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? "\"" + value() + "\"" : value();
    }
};

CHILD_POINTER_DEFINITION(Text, Element);

CHILD_END

#endif // CHILD_TEXT_H
