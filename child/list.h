#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "child/exception.h"
#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(List, Object);

#define CHILD_LIST(...) ListPtr(new List(__VA_ARGS__))

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

class List : public Object {
    CHILD_DECLARATION(List, Object);
public:
    List(const NodeList &other = NodeList(), // default constructor
         const ListPtr &origin = find("Object")->child("List")) : Object(origin), _list(NULL) {
        if(!other.isEmpty()) foreach(NodePtr node, other) append(node);
    }

    List(const ObjectPtr &origin) : Object(origin), _list(NULL) {} // root constructor

    List(const ListPtr &origin) : Object(origin), _list(NULL) { // fork constructor
        if(origin->isNotEmpty()) foreach(NodePtr node, *origin->_list) append(node->fork());
    }

    List(const List &other) : Object(other), _list(NULL) { // copy constructor
        if(other.isNotEmpty()) foreach(NodePtr node, *other._list) append(node);
    }

    virtual ~List() {
        if(_list) {
            foreach(NodePtr node, *_list) removeAnonymousChild(node);
            delete _list;
        }
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

CHILD_END

#endif // CHILD_LIST_H
