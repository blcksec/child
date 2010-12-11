#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "child/object.h"

CHILD_BEGIN

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

template<class C, class T>
class GenericList : public Object {
public:
    GenericList(const NodePtr &origin, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) {}

    GenericList(const NodePtr &origin, const T &value, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value); }

    GenericList(const NodePtr &origin, const T &value1, const T &value2, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value1); append(value2); }

    GenericList(const NodePtr &origin, const T &value1, const T &value2, const T &value3, const bool isBunched = false) :
        Object(origin), _list(NULL), _isBunched(isBunched) { append(value1); append(value2); append(value3); }

    GenericList(const NodePtr &origin, const QList<T> &other, const bool isBunched = false) :
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

    C initFork() {
        C orig = origin();
        if(orig->isNotEmpty()) {
            foreach(T node, *orig->_list) _append(node->fork());
            hasChanged();
        }
        return C(this);
    }

    T insert(int i, const T &value) { _insert(i, value); hasChanged(); return value; }

    void _insert(int i, const T &value) {
        checkIndex(i, true);
        CHILD_CHECK_VALUE(value);
        if(!_list) { _list = new QList<T>; }
        _list->insert(i, value);
        addAnonymousChild(value);
    }

    C insert(int i, const C &otherList) {
        checkIndex(i, true);
        if(!otherList) CHILD_THROW(NullPointerException, "ListPtr is NULL");
        for(int j = 0; j < otherList->size(); j++) {
            _insert(i + j, otherList->get(j));
        }
        hasChanged();
        return otherList;
    }

    T append(const T &value) { return insert(size(), value); }
    T _append(const T &value) { _insert(size(), value); return value; }
    C append(const C &otherList) { return insert(size(), otherList); }
    T prepend(const T &value) { return insert(0, value); }
    C prepend(const C &otherList) { return insert(0, otherList); }

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

    bool hasValue(const T &value) const {
        CHILD_CHECK_VALUE(value);
        if(_list)
            foreach(T node, *_list)
                if(node->compare(*value) == Equal) return true;
        return false;
    }

    void clear() {
        if(_list) {
            foreach(NodePtr node, *_list) removeAnonymousChild(node);
            _list->clear();
            hasChanged();
        }
    }

    int size() const { return _list ? _list->size() : 0; }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

    const QString join(const QString &separator = "", const QString &prefix = "",
                       const QString &suffix = "", bool debug = false) const {
        QString str;
        if(_list) {
            bool first = true;
            Iterator i(C(this));
            while(T value = i.next()) {
                if(!first) str += separator; else first = false;
                str += prefix + value->toString(debug) + suffix;
            }
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

    virtual const QString toString(bool debug = false) const { return "[" + join(", ", "", "", debug) + "]"; }

    class Iterator {
    public:
        Iterator(const C &list) : _iterator(QListIterator<T>(*list->_list)) {}

        bool hasNext() const { return _iterator.hasNext(); }
        const T &next() { return hasNext() ? _iterator.next() : T::null(); }
    private:
        QListIterator<T> _iterator;
    };

//    class const_iterator {
//    public:
//        const_iterator(const List *list, int i = 0) : _list(list), _i(i) {}
//        NodePtr const operator*() const { return _list->get(_i); }
//        const_iterator &operator++() { ++_i; return *this; }
//        bool operator!=(const const_iterator &o) const { return _i != o._i; }
//    private:
//        const List *_list;
//        int _i;
//    };

//    const_iterator begin() const { return const_iterator(this); }
//    const_iterator end() const { return const_iterator(this, size()); }
private:
    QList<T> *_list;
    bool _isBunched;
};

CHILD_PTR_DECLARATION(List, Object);

#define CHILD_LIST(ARGS...) ListPtr(new List(Node::context()->child("Object", "List"), ##ARGS))

class List : public GenericList<ListPtr, NodePtr> {
    CHILD_DECLARATION(List, Object);
public:
    List(const NodePtr &origin, const NodeList &other = NodeList()) :
        GenericList<ListPtr, NodePtr>(origin, other) {}

    List(const NodePtr &origin, const QList<NodeRef> &other) :
        GenericList<ListPtr, NodePtr>(origin) {
        if(!other.isEmpty()) {
            foreach(NodeRef ref, other) _append(ref);
            hasChanged();
        }
    }

    List(const List &other) : GenericList<ListPtr, NodePtr>(other) {}

    static void initRoot() { Object::root()->addChild("List", root()); }

    virtual NodePtr fork() const {
        ListPtr list = new List(this);
        list->initFork();
        return list;
    }
};

CHILD_PTR_DEFINITION(List, Object);

CHILD_END

#endif // CHILD_LIST_H
