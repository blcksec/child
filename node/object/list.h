#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

template<class T>
class GenericList : public Object {
public:
    explicit GenericList(Node *origin, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) {}

    GenericList(Node *origin, const T &value, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value); }

    GenericList(Node *origin, const T &value1, const T &value2, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value1); append(value2); }

    GenericList(Node *origin, const T &value1, const T &value2, const T &value3, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value1); append(value2); append(value3); }

    GenericList(Node *origin, const QList<T> &other, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) {
        if(!other.isEmpty()) {
            foreach(T node, other) _append(node);
            hasChanged();
        }
    }

    GenericList(const GenericList &other) : Object(other), _list(NULL), _isBunched(other._isBunched) {
        if(other.isNotEmpty()) {
            foreach(T node, *other._list) _append(node);
            hasChanged();
        }
    }

    virtual ~GenericList() {
        if(_list) {
            foreach(T node, *_list) removeAnonymousChild(node);
            delete _list;
        }
    }

    virtual void initFork() {
        Object::initFork();
        GenericList *orig = static_cast<GenericList *>(origin());
        if(orig->isNotEmpty()) {
            foreach(T node, *orig->_list) _append(node->fork());
            hasChanged();
        }
    }

    T insert(int i, const T &value) { _insert(i, value); hasChanged(); return value; }

    void _insert(int i, const T &value) {
        checkIndex(i, true);
        CHILD_CHECK_VALUE(value);
        if(!_list) { _list = new QList<T>; }
        _list->insert(i, value);
        addAnonymousChild(value);
    }

    const GenericList *insert(int i, const GenericList *otherList) {
        checkIndex(i, true);
        if(!otherList) CHILD_THROW(NullPointerException, "List pointer is NULL");
        for(int j = 0; j < otherList->size(); j++) {
            _insert(i + j, otherList->get(j));
        }
        hasChanged();
        return otherList;
    }

    T append(const T &value) { return insert(size(), value); }
    T _append(const T &value) { _insert(size(), value); return value; }
    const GenericList *append(const GenericList *otherList) { return insert(size(), otherList); }
    T prepend(const T &value) { return insert(0, value); }
    const GenericList *prepend(const GenericList *otherList) { return insert(0, otherList); }

    T get(int i) const {
        checkIndex(i);
        return _list->at(i);
    }

    T first() const { return get(0); }
    T second() const { return get(1); }
    T third() const { return get(2); }
    T fourth() const { return get(3); }
    T fifth() const { return get(4); }
    T last() const { return get(size()-1); }

    T set(int i, const T &value) {
        checkIndex(i);
        CHILD_CHECK_VALUE(value);
        removeAnonymousChild(_list->at(i));
        _list->replace(i, value);
        addAnonymousChild(value);
        hasChanged();
        return value;
    }

    void remove(int i) {
        checkIndex(i);
        removeAnonymousChild(_list->at(i));
        _list->removeAt(i);
        hasChanged();
    }

    bool hasIndex(int i) { return i >= 0 && i < size(); }

    bool hasValue(const T &value) const {
        CHILD_CHECK_VALUE(value);
        if(_list)
            foreach(T node, *_list)
                if(node->isEqualTo(value)) return true;
        return false;
    }

    void clear() {
        if(_list) {
            foreach(Node *node, *_list) removeAnonymousChild(node);
            _list->clear();
            hasChanged();
        }
    }

    int size() const { return _list ? _list->size() : 0; }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

    const QString join(const QString &separator = "", const QString &prefix = "",
                       const QString &suffix = "", bool debug = false, short level = 0) const {
        QString str;
        bool first = true;
        Iterator i(this);
        while(T value = i.next()) {
            if(!first) str += separator; else first = false;
            str += prefix + value->toString(debug, level) + suffix;
        }
        return str;
    }

    void checkIndex(int i, bool insertMode = false) const {
        int max = size();
        if(!insertMode) max--;
        if(i < 0) CHILD_THROW(IndexOutOfBoundsException, "index is less than zero");
        if(i > max) {
            if(insertMode)
                CHILD_THROW(IndexOutOfBoundsException, "index is greater than size");
            else
                CHILD_THROW(IndexOutOfBoundsException, "index is greater than size-1");
        }
    }

    virtual void hasChanged() {}

    virtual QString toString(bool debug = false, short level = 0) const {
        return "[" + join(", ", "", "", debug, level) + "]";
    }
private:
    QList<T> *_list;
    bool _isBunched;
public:
    class Iterator {
    public:
        Iterator(const GenericList *list) : _iterator(list->_list ? new QListIterator<T>(*list->_list) : NULL) {}
        ~Iterator() { delete _iterator; }

        bool hasNext() const { return _iterator && _iterator->hasNext(); }
        const T next() { return hasNext() ? _iterator->next() : NULL; }
    private:
        QListIterator<T> *_iterator;
    };
};

#define CHILD_LIST(ARGS...) new List(context()->child("Object", "List"), ##ARGS)

class List : public GenericList<Node *> {
    CHILD_DECLARE(List, Object, Object);
public:
    explicit List(Node *origin, const QList<Node *> &other = QList<Node *>()) :
        GenericList<Node *>(origin, other) {}

    CHILD_FORK_METHOD(List);
};

CHILD_END

#endif // CHILD_LIST_H
