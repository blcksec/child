#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

//#include "child/nativemethod.h"
#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Text, Object);

#define CHILD_TEXT(ARGS...) TextPtr(new Text(Node::findInContext("Object")->child("Text"), ##ARGS))

class Text : public Object {
    CHILD_DECLARATION(Text, Object);
public:
    Text(const NodePtr &origin, const QString &value = "") : Object(origin), _value(value) {}
//    Text(const Text &other) : Object(other), _value(other._value) {}
    static void initRoot() { Object::root()->addChild("Text", root()); }
    virtual NodePtr fork() const { return new Text(this, _value); }

    const QString value() const { return _value; }
    void setValue(const QString &value) { _value = value; }

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

    virtual uint hash() const { return qHash(value()); }

    static QString unescapeSequence(const QString &source);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual const QString inspect() const { return QString("\"%1\"").arg(value()); }
private:
    QString _value;
};

CHILD_PTR_DEFINITION(Text, Object);

CHILD_END

#endif // CHILD_TEXT_H
