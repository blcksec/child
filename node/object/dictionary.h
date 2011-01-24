#ifndef CHILD_DICTIONARY_H
#define CHILD_DICTIONARY_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_CHECK_KEY(KEY) \
if(!(KEY)) CHILD_THROW(NullPointerException, "key is NULL")

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

template<class K, class V>
class GenericDictionary : public Object {
public:
    explicit GenericDictionary(Node *origin, const QHash<K, V> &other = (QHash<K, V>())) : Object(origin), _hash(NULL) {
        if(!other.isEmpty()) {
            QHashIterator<K, V> i(other);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()); }
        }
    }

    GenericDictionary(const GenericDictionary &other) : Object(other), _hash(NULL) {
        if(other.isNotEmpty()) {
            QHashIterator<K, V> i(*other._hash);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()); }
        }
    }

    virtual ~GenericDictionary() {
        if(_hash) {
            foreach(V value, *_hash) removeAnonymousChild(value);
            delete _hash;
        }
    }

    virtual void initFork() {
        Object::initFork();
        GenericDictionary *orig = static_cast<GenericDictionary *>(origin());
        if(orig->isNotEmpty()) {
            QHashIterator<K, V> i(*orig->_hash);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()->fork()); }
        }
    }

    V get(const K &key) const {
        CHILD_CHECK_KEY(key);
        V value;
        if(!(value = hasKey(key))) CHILD_THROW(NotFoundException, "key not found");
        return value;
    }

    V set(const K &key, const V &value) {
        CHILD_CHECK_KEY(key);
        CHILD_CHECK_VALUE(value);
        if(!_hash) { _hash = new QHash<K, V>; }
        if(V oldValue = _hash->value(key)) removeAnonymousChild(oldValue);
        _hash->insert(key, value);
        addAnonymousChild(value);
        return value;
    }

//        Node *key(Node *value) const;
    V value(const K &key) const { return get(key); }
    int size() const { return _hash ? _hash->size() : 0; }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

    QList<K> keys() const {
        QList<K> list;
        if(_hash) foreach(K key, _hash->keys()) list.append(key);
        return list;
    }
//        NodeList values() const;
    V hasKey(const K &key) const { CHILD_CHECK_KEY(key); return _hash ? _hash->value(key) : V(); }
//        bool hasValue(Node *value) const;

    void remove(const K &key) {
        CHILD_CHECK_KEY(key);
        V oldValue;
        if(!(_hash && (oldValue = _hash->value(key)))) CHILD_THROW(NotFoundException, "key not found");
        removeAnonymousChild(oldValue);
        _hash->remove(key);
    }

    void clear() {
        foreach(K key, keys()) remove(key);
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str = "[";
        bool first = true;
        foreach(K key, keys()) {
            if(!first) str += ", "; else first = false;
            str += QString("%1: %2").arg(key->toString(debug, level), get(key)->toString(debug, level));
        }
        str.append("]");
        return str;
    }
private:
    QHash<K, V> *_hash;
public:
    class Iterator {
    public:
        Iterator(const GenericDictionary *dict) : _iterator(dict->_hash ? new QHashIterator<K, V>(*dict->_hash) : NULL) {}
        ~Iterator() { delete _iterator; }

        bool hasNext() const { return _iterator && _iterator->hasNext(); }
        const V next() { return hasNext() ? _iterator->next().value() : NULL; }

        const K key() { return _iterator->key(); }
        const V value() { return _iterator->value(); }
    private:
        QHashIterator<K, V> *_iterator;
    };
};

#define CHILD_DICTIONARY(ARGS...) new Dictionary(context()->child("Object", "Dictionary"), ##ARGS)

class Dictionary : public GenericDictionary<Node::Reference, Node *> {
    CHILD_DECLARE(Dictionary, Object, Object);
public:
    explicit Dictionary(Node *origin) :
        GenericDictionary<Node::Reference, Node *>(origin) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Dictionary);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Dictionary);

    CHILD_DECLARE_NATIVE_METHOD(init);

    CHILD_DECLARE_NATIVE_METHOD(get);

    CHILD_DECLARE_NATIVE_METHOD(size);
    CHILD_DECLARE_NATIVE_METHOD(empty);
};

CHILD_END

#endif // CHILD_DICTIONARY_H
