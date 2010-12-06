#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "child/exception.h"
#include "child/object.h"

CHILD_BEGIN

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

template<class P, class T>
class GenericList : public Object {
public:
    GenericList(const NodePtr &origin, const QList<T> &other = QList<T>()) : Object(origin), _list(NULL) {
        if(!other.isEmpty()) {
            foreach(T node, other) _append(node);
            hasChanged();
        }
    }

    GenericList(const GenericList &other) : Object(other), _list(NULL) {
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

    void initFork() {
        P orig = origin();
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

    P insert(int i, const P &otherList) {
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
    P append(const P &otherList) { return insert(size(), otherList); }
    T prepend(const T &value) { return insert(0, value); }
    P prepend(const P &otherList) { return insert(0, otherList); }

    T get(int i) const {
        checkIndex(i);
        return _list->at(i);
    }

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

    virtual const QString inspect() const {
        QString str = "[";
        bool first = true;
        for(int i = 0; i < size(); i++) {
            if(!first) str.append(", "); else first = false;
            str.append(get(i)->inspect());
        }
        str.append("]");
        return str;
    }

    class Iterator {
    public:
        Iterator(const P &list) : _iterator(QListIterator<T>(*list->_list)) {}

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
    QList<T> *_list;
    private:
};

CHILD_PTR_DECLARATION(List, Object);

#define CHILD_LIST(ARGS...) ListPtr(new List(Node::findInContext("Object")->child("List"), ##ARGS))

class List : public GenericList<ListPtr, NodePtr> {
    CHILD_DECLARATION(List, Object);
public:
    List(const NodePtr &origin, const NodeList &other = NodeList()) :
        GenericList<ListPtr, NodePtr>(origin, other) {}
    List(const List &other) : GenericList<ListPtr, NodePtr>(other) {}

    static void initRoot() { Object::root()->addChild("List", root()); }

    virtual NodePtr fork() const {
        ListPtr list = new List(this);
        list->initFork();
        return list;
    }
};

CHILD_PTR_DEFINITION(List, Object);


/*
CHILD_PTR_DECLARATION(List, Object);

#define CHILD_LIST(ARGS...) ListPtr(new List(Node::findInContext("Object")->child("List"), ##ARGS))

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

class List : public Object {
    CHILD_DECLARATION(List, Object);
public:
    List(const NodePtr &origin, const NodeList &other = NodeList()) : Object(origin), _list(NULL) {
        if(!other.isEmpty()) foreach(NodePtr node, other) append(node);
    }

    List(const List &other) : Object(other), _list(NULL) {
        if(other.isNotEmpty()) foreach(NodePtr node, *other._list) append(node);
    }

    virtual ~List() {
        if(_list) {
            foreach(NodePtr node, *_list) removeAnonymousChild(node);
            delete _list;
        }
    }

    static void initRoot() { Object::root()->addChild("List", root()); }

    virtual NodePtr fork() const {
        ListPtr list(new List(NodePtr(this)));
        if(isNotEmpty()) foreach(NodePtr node, *_list) list->append(node->fork());
        return list;
    }

    NodePtr insert(int i, const NodePtr &value) {
        checkIndex(i, true);
        CHILD_CHECK_VALUE(value);
        if(!_list) { _list = new NodeList; }
        _list->insert(i, value);
        addAnonymousChild(value);
        return value;
    }

    ListPtr insert(int i, const ListPtr &otherList) {
        checkIndex(i, true);
        if(!otherList) CHILD_THROW(NullPointerException, "ListPtr is NULL");
        for(int j = 0; j < otherList->size(); j++) {
            insert(i + j, otherList->get(j));
        }
        return otherList;
    }

    NodePtr append(const NodePtr &value) { return insert(size(), value); }
    ListPtr append(const ListPtr &otherList) { return insert(size(), otherList); }
    NodePtr prepend(const NodePtr &value) { return insert(0, value); }
    ListPtr prepend(const ListPtr &otherList) { return insert(0, otherList); }

    NodePtr get(int i) const {
        checkIndex(i);
        return _list->at(i);
    }

    NodePtr set(int i, const NodePtr &value) {
        checkIndex(i);
        CHILD_CHECK_VALUE(value);
        removeAnonymousChild(_list->at(i));
        _list->replace(i, value);
        addAnonymousChild(value);
        return value;
    }

    void remove(int i) {
        checkIndex(i);
        removeAnonymousChild(_list->at(i));
        _list->removeAt(i);
    }

    bool hasValue(const NodePtr &value) const {
        CHILD_CHECK_VALUE(value);
        if(_list)
            foreach(NodePtr node, *_list)
                if(node->compare(*value) == Equal) return true;
        return false;
    }

    ListPtr clear() {
        if(_list) {
            foreach(NodePtr node, *_list) removeAnonymousChild(node);
            _list->clear();
        }
        return ListPtr(this);
    }

    int size() const { return _list ? _list->size() : 0; }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

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

    virtual const QString inspect() const {
        QString str = "[";
        bool first = true;
        for(int i = 0; i < size(); i++) {
            if(!first) str.append(", "); else first = false;
            str.append(get(i)->inspect());
        }
        str.append("]");
        return str;
    }

    class const_iterator {
    public:
        const_iterator(const List *list, int i = 0) : _list(list), _i(i) {}
        NodePtr const operator*() const { return _list->get(_i); }
        const_iterator &operator++() { ++_i; return *this; }
        bool operator!=(const const_iterator &o) const { return _i != o._i; }
    private:
        const List *_list;
        int _i;
    };

    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const { return const_iterator(this, size()); }
private:
    NodeList *_list;
};

CHILD_PTR_DEFINITION(List, Object);
*/

CHILD_END

#endif // CHILD_LIST_H
