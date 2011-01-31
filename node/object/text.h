#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "node/object/element.h"
#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/message.h"

CHILD_BEGIN

#define CHILD_TEXT(ARGS...) new Text(context()->child("Object", "Text"), ##ARGS)

class Text : public GenericElement<QString> {
    CHILD_DECLARE(Text, Element, Object);
public:
    explicit Text(Node *origin, const QString &value = "", bool isTranslatable = false,
                  QList<IntPair> *interpolableSlices = NULL);

    Text(const Text &other);

    virtual ~Text() { delete _interpolableSlices; }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Text);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Text, value(), isTranslatable(), interpolableSlices());

    CHILD_DECLARE_NATIVE_METHOD(init);

    bool isTranslatable() const { return _isTranslatable; }
    void setIsTranslatable(bool isTranslatable) { _isTranslatable = isTranslatable; }

    QList<IntPair> *interpolableSlices() const { return _interpolableSlices; }

    void setInterpolableSlices(QList<IntPair> *interpolableSlices) {
        _interpolableSlices = interpolableSlices && !interpolableSlices->isEmpty() ?
                    new QList<IntPair>(*interpolableSlices) : NULL;
    }

    virtual Node *run(Node *receiver = context());

    CHILD_DECLARE_NATIVE_METHOD(concatenate);
    CHILD_DECLARE_NATIVE_METHOD(multiply);

    CHILD_DECLARE_NATIVE_METHOD(uppercase);
    CHILD_DECLARE_NATIVE_METHOD(lowercase);

    static QString capitalize(QString text);
    CHILD_DECLARE_NATIVE_METHOD(capitalize);

    CHILD_DECLARE_NATIVE_METHOD(size);
    CHILD_DECLARE_NATIVE_METHOD(empty);

    virtual bool isEqualTo(const Node *other) const;
    CHILD_DECLARE_NATIVE_METHOD(equal_to);

    virtual short compare(const Node *other) const;
    short compare(const QString &other) const;
    CHILD_DECLARE_NATIVE_METHOD(compare);

    static QString unescapeSequence(const QString &source, QList<IntPair> *interpolableSlices = NULL);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual double toDouble(bool *okPtr = NULL) const;
    virtual QChar toChar() const;
    virtual QString toString(bool debug = false, short level = 0) const;
private:
    bool _isTranslatable;
    QList<IntPair> *_interpolableSlices;
public:
    #define CHILD_TEXT_ITERATOR(ARGS...) new Iterator(context()->child("Object", "Text", "Iterator"), ##ARGS)

    #define CHILD_CHECK_TEXT_ITERATOR_INDEX \
    if(!hasValue()) CHILD_THROW(IndexOutOfBoundsException, "Iterator is out of bounds");

    class Iterator : public Object {
        CHILD_DECLARE(Iterator, Object, Text);
    public:
        explicit Iterator(Node *origin, Text *text = NULL, int index = 0) :
            Object(origin), _text(text), _index(index) {}

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Iterator);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Iterator, _text, _index);

        CHILD_DECLARE_NATIVE_METHOD(init);

        QChar value() const {
            CHILD_CHECK_TEXT_ITERATOR_INDEX;
            return _text->value().at(_index);
        }

        CHILD_DECLARE_NATIVE_METHOD(value);

        bool hasValue() const {
            if(!_text) CHILD_THROW(NullPointerException, "Text pointer is NULL");
            return _index >= 0 && _index < _text->value().size();
        }

        void first() { _index = 0; }
        CHILD_DECLARE_NATIVE_METHOD(first);

        void last() {
            if(!_text) CHILD_THROW(NullPointerException, "Text pointer is NULL");
            _index = _text->value().size() - 1;
        }

        CHILD_DECLARE_NATIVE_METHOD(last);

        void next() { CHILD_CHECK_TEXT_ITERATOR_INDEX; _index++; }
        CHILD_DECLARE_NATIVE_METHOD(prefix_increment);

        void previous() { CHILD_CHECK_TEXT_ITERATOR_INDEX; _index--; }
        CHILD_DECLARE_NATIVE_METHOD(prefix_decrement);
    private:
        Text *_text;
        int _index;
    };
};

CHILD_END

#endif // CHILD_TEXT_H
