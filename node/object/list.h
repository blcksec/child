#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

// === GenericAbstractList ===

template<class T>
class GenericAbstractList : public Object {
public:
    explicit GenericAbstractList(Node *origin) : Object(origin) {}

    virtual T get(int i) const {
        Q_UNUSED(i);
        CHILD_THROW(RuntimeException, "abstract method called");
    }

    T first() const { return get(0); }
    T second() const { return get(1); }
    T third() const { return get(2); }
    T fourth() const { return get(3); }
    T fifth() const { return get(4); }
    T last() const { return get(size()-1); }

    virtual T set(int i, const T &value)  {
        Q_UNUSED(i);
        Q_UNUSED(value);
        CHILD_THROW(RuntimeException, "abstract method called");
    }

    T insert(int i, const T &value) {
        silentlyInsert(i, value);
        hasChanged();
        return value;
    }

    virtual void silentlyInsert(int i, const T &value)  {
        Q_UNUSED(i);
        Q_UNUSED(value);
        CHILD_THROW(RuntimeException, "abstract method called");
    }

    const GenericAbstractList *insert(int i, const GenericAbstractList *otherList) {
        checkIndex(i, true);
        if(!otherList) CHILD_THROW(NullPointerException, "List pointer is NULL");
        for(int j = 0; j < otherList->size(); j++) {
            silentlyInsert(i + j, otherList->get(j));
        }
        hasChanged();
        return otherList;
    }

    T append(const T &value) { return insert(size(), value); }
    void silentlyAppend(const T &value) { silentlyInsert(size(), value); }
    const GenericAbstractList *append(const GenericAbstractList *otherList) { return insert(size(), otherList); }
    T prepend(const T &value) { return insert(0, value); }
    const GenericAbstractList *prepend(const GenericAbstractList *otherList) { return insert(0, otherList); }

    virtual void remove(int i)  {
        Q_UNUSED(i);
        CHILD_THROW(RuntimeException, "abstract method called");
    }
    virtual void clear()  {
        CHILD_THROW(RuntimeException, "abstract method called");
    }

    bool hasIndex(int i) { return i >= 0 && i < size(); }

    bool hasValue(const T &value) const {
        CHILD_CHECK_VALUE(value);
        for(int i = 0; i < size(); ++i)
            if(get(i)->isEqualTo(value))
                return true;
        return false;
    }

    virtual int size() const  {
        CHILD_THROW(RuntimeException, "abstract method called");
    }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

    const QString join(const QString &separator = "", const QString &prefix = "",
                       const QString &suffix = "", bool debug = false, short level = 0) const {
        QString str;
        for(int i = 0; i < size(); ++i) {
            if(i > 0) str += separator;
            str += prefix + get(i)->toString(debug, level) + suffix;
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
};

// === AbstractList ===

#define CHILD_ABSTRACT_LIST(ARGS...) new AbstractList(context()->child("Object", "AbstractList"), ##ARGS)

class AbstractList : public GenericAbstractList<Node *> {
    CHILD_DECLARE(AbstractList, Object, Object);
public:
    explicit AbstractList(Node *origin) : GenericAbstractList<Node *>(origin) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(AbstractList);

    CHILD_DECLARE_NATIVE_METHOD(get);

    CHILD_DECLARE_NATIVE_METHOD(append);

    CHILD_DECLARE_NATIVE_METHOD(size);
    CHILD_DECLARE_NATIVE_METHOD(empty_qm);
};

// === GenericList ===

template<class T>
class GenericList : public GenericAbstractList<T> {
public:
    using GenericAbstractList<T>::origin;
    using GenericAbstractList<T>::removeAnonymousChild;
    using GenericAbstractList<T>::checkIndex;
    using GenericAbstractList<T>::hasChanged;

    explicit GenericList(Node *origin, bool isBunch = false) :
        GenericAbstractList<T>(origin), _list(NULL), _isBunch(isBunch) {}

    GenericList(Node *origin, const T &value, bool isBunch = false) :
        GenericAbstractList<T>(origin), _list(NULL), _isBunch(isBunch) { append(value); }

    GenericList(Node *origin, const T &value1, const T &value2, bool isBunch = false) :
        GenericAbstractList<T>(origin), _list(NULL), _isBunch(isBunch) { append(value1); append(value2); }

    GenericList(Node *origin, const T &value1, const T &value2, const T &value3, bool isBunch = false) :
        GenericAbstractList<T>(origin), _list(NULL), _isBunch(isBunch) { append(value1); append(value2); append(value3); }

    GenericList(Node *origin, const QList<T> &other, bool isBunch = false) :
        GenericAbstractList<T>(origin), _list(NULL), _isBunch(isBunch) {
        if(!other.isEmpty()) {
            foreach(T node, other) silentlyAppend(node);
            hasChanged();
        }
    }

    GenericList(const GenericList &other) : GenericAbstractList<T>(other), _list(NULL), _isBunch(other._isBunch) {
        if(other.isNotEmpty()) {
            foreach(T node, *other._list) silentlyAppend(node);
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
            foreach(T node, *orig->_list) silentlyAppend(node->fork());
            hasChanged();
        }
    }

    virtual T get(int i) const {
        checkIndex(i);
        return _list->at(i);
    }

    virtual T set(int i, const T &value) {
        checkIndex(i);
        CHILD_CHECK_VALUE(value);
        removeAnonymousChild(_list->at(i));
        _list->replace(i, value);
        addAnonymousChild(value);
        hasChanged();
        return value;
    }

    virtual void silentlyInsert(int i, const T &value) {
        checkIndex(i, true);
        CHILD_CHECK_VALUE(value);
        if(!_list) { _list = new QList<T>; }
        _list->insert(i, value);
        addAnonymousChild(value);
    }

    virtual void remove(int i) {
        checkIndex(i);
        removeAnonymousChild(_list->at(i));
        _list->removeAt(i);
        hasChanged();
    }

    virtual void clear() {
        if(_list) {
            foreach(Node *node, *_list) removeAnonymousChild(node);
            _list->clear();
            hasChanged();
        }
    }

    virtual int size() const { return _list ? _list->size() : 0; }
private:
    QList<T> *_list;
    bool _isBunch;
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

// === List ===

#define CHILD_LIST(ARGS...) new List(context()->child("Object", "List"), ##ARGS)

class List : public GenericList<Node *> {
    CHILD_DECLARE(List, AbstractList, Object);
public:
    explicit List(Node *origin) : GenericList<Node *>(origin) {}

    List(Node *origin, const QList<Node *> &other) : GenericList<Node *>(origin, other) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(List);

    CHILD_DECLARE_NATIVE_METHOD(init);
};

// === GenericVirtualList ===

template<class T>
class GenericVirtualList : public GenericAbstractList<T> {
public:
    using GenericAbstractList<T>::checkIndex;
    using GenericAbstractList<T>::hasChanged;

    explicit GenericVirtualList(Node *origin, QList<T> **source = NULL) : GenericAbstractList<T>(origin), _source(source) {}

    GenericVirtualList(const GenericVirtualList &other) : GenericAbstractList<T>(other), _source(other._source) {}

    QList<T> **source() const { return _source; }
    void setSource(QList<T> **source) { _source = source; }

    virtual T get(int i) const {
        checkIndex(i);
        return (*_source)->at(i);
    }

    virtual T set(int i, const T &value) {
        checkIndex(i);
        CHILD_CHECK_VALUE(value);
        (*_source)->replace(i, value);
        hasChanged();
        return value;
    }

    virtual void silentlyInsert(int i, const T &value) {
        checkIndex(i, true);
        CHILD_CHECK_VALUE(value);
        if(!*_source) { *_source = new QList<T>; }
        (*_source)->insert(i, value);
    }

    virtual void remove(int i) {
        checkIndex(i);
        (*_source)->removeAt(i);
        hasChanged();
    }

    virtual void clear() {
        if(*_source) {
            (*_source)->clear();
            hasChanged();
        }
    }

    virtual int size() const { return *_source ? (*_source)->size() : 0; }
private:
    QList<T> **_source;
};

// === VirtualList ===

#define CHILD_VIRTUAL_LIST(ARGS...) new VirtualList(context()->child("Object", "VirtualList"), ##ARGS)

class VirtualList : public GenericVirtualList<Node *> {
    CHILD_DECLARE(VirtualList, AbstractList, Object);
public:
    explicit VirtualList(Node *origin, QList<Node *> **source = NULL) : GenericVirtualList<Node *>(origin, source) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(VirtualList, source());
};

CHILD_END

#endif // CHILD_LIST_H
