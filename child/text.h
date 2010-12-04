#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

//#include "child/nativemethod.h"
#include "child/object.h"

namespace Child {

CHILD_PTR_DECLARATION(Text, Object);

class Text : public Object {
    CHILD_DECLARATION(Text, Object);
public:
    Text(const QString &value = "", const TextPtr &origin = find("Object")->child("Text")) : // default constructor
        Object(origin), _value(value) {}

    Text(const ObjectPtr &origin) : Object(origin) {} // root constructor

    Text(const TextPtr &origin) : Object(origin), _value(origin->_value) {} // fork constructor

    const QString value() const { return _value; }
    TextPtr setValue(const QString &value) { _value = value; return TextPtr(this); }

    TextPtr upcase() { return TextPtr(new Text(value().toUpper())); }

    virtual Comparison compare(const NodePtr &other) const {
        if(NodePtr(this) == other) return Equal;
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

} // namespace Child

#endif // CHILD_TEXT_H
