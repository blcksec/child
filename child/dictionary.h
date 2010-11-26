#ifndef CHILD_DICTIONARY_H
#define CHILD_DICTIONARY_H

#include "child/nativemethod.h"
#include "child/object.h"

namespace Child {
    class NewDictionary : public Object {
        CHILD_DECLARATION(NewDictionary, Object, Object);
    public:
        NewDictionary() : _hash(NULL), _nextNumberId(firstNumberId) {}

        virtual ~NewDictionary() {
            delete _hash;
        }

        virtual void initFork() {
            NewDictionary *orig = NewDictionary::as(origin());
            if(orig->_hash) _hash = new NumberIdHash(*orig->_hash); // TODO: optimized fork!
            _nextNumberId = orig->_nextNumberId;
        }

        Node *get(Node *key) const {
            NumberId id = getNumberId(key);
            if(id == nullNumberId) throw NotFoundException("key not found");
            return child(numberIdToName(id));
        }

        Node *set(Node *key, Node *value) {
            checkValue(value);
            NumberId id = getNumberId(key);
            if(id == nullNumberId)
                addDirectChild(numberIdToName(id = nextNumberId()), value);
            else
                setChild(numberIdToName(id), value);
            if(!_hash) { _hash = new NumberIdHash; }
            _hash->insert(key, id);
            return value;
        }

//        Node *key(Node *value) const;
        Node *value(Node *key) const { return get(key); }
        int size() const { return _hash ? _hash->size() : 0; }
        bool isEmpty() const { return size() == 0; }
        bool isNotEmpty() const { return size() > 0; }

        NodeList keys() const {
            NodeList list;
            if(_hash) foreach(NodeRef nodeRef, _hash->keys()) list.append(nodeRef.node);
            return(list);
        }
//        NodeList values() const;
        bool hasKey(Node *key) const { return getNumberId(key) != nullNumberId; }
//        bool hasValue(Node *value) const;

        NewDictionary *remove(Node *key) {
            NumberId id = getNumberId(key);
            if(id == nullNumberId) throw NotFoundException("key not found");
            _hash->remove(key); // FIXME: what about the key? Memory leak?
            removeDirectChild(numberIdToName(id)); // FIXME: should use RemoveChild
            return this;
        }

        NewDictionary *clear() {
            foreach(Node *key, keys()) remove(key);
            return this;
        }

        virtual const QString inspect() const {
            QString str = "[";
            bool first = true;
            foreach(Node *key, keys()) {
                if(!first) str.append(", "); else first = false;
                str.append(QString("%1: %2").arg(key->inspect(), get(key)->inspect()));
            }
            str.append("]");
            return str;
        }
    private:
        NumberIdHash *_hash;
        NumberId _nextNumberId;

        NumberId getNumberId(Node *key) const {
            checkKey(key);
            return _hash ? _hash->value(key, nullNumberId) : nullNumberId;
        }

        const NumberId nextNumberId() { return(_nextNumberId++); }

        void checkKey(Node *key) const {
            if(!key) throw NullPointerException("key pointer is NULL");
        }

        void checkValue(Node *value) const {
            if(!value) throw NullPointerException("value pointer is NULL");
        }
    };



 ///////////// /////////////   /////////////   /////////////   /////////////   /////////////   /////////////



    class Dictionary : public Object {
        CHILD_DECLARATION(Dictionary, Object, Object);
    public:
        Dictionary() : _hash(NULL), _addedKeys(NULL), _removedKeys(NULL), _size(0), _anonymousKeyCount(0) {}

        virtual ~Dictionary() {
            delete _hash;
            delete _addedKeys;
            delete _removedKeys;
        }

        virtual void initFork() {
            Dictionary *orig = Dictionary::as(origin());
            _size = orig->_size;
            _anonymousKeyCount = orig->_anonymousKeyCount;
        }

        Node *get(QString key) {
            checkKey(key);
            escapeKey(key);
            const Dictionary *dict = this;
            Node *value = NULL;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) {
                    value = dict->_hash->value(key);
                    break;
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            if(!value) throw NotFoundException("key not found");
            if(dict != this) {
                value = value->fork()->setIsVirtual(true);
                if(!_hash) { _hash = new NodeHash; }
                _hash->insert(key, value);
                addDirectChild(_keyToName(key), value);
            }
            return value;
        }

        Node *get(int i) { // TODO: optimize
            if(i < 0) throw IndexOutOfBoundsException("index is less than zero");
            if(i >= size()) throw IndexOutOfBoundsException("index is greater than size-1");
            return namedNodes().at(i).node;
        }

        Node *set(QString key, Node *value) {
            if(key.isEmpty()) {
                key = QString("\\%1").arg(_anonymousKeyCount);
                _anonymousKeyCount++;
            } else {
                escapeKey(key);
            }
            checkValue(value);
            if(!_hasKey(key)) {
                _size++;
                if(!_addedKeys) { _addedKeys = new QList<QString>; }
                _addedKeys->append(key);
            }
            if(!_hash) { _hash = new NodeHash; }
            _hash->insert(key, value);
            addOrSetDirectChild(_keyToName(key), value);
            return value;
        }

        const QString &key(Node *value) const;
        // cherche value ou ancètre de value ?

        Node *value(const QString &key) { return get(key); }
        int size() const { return _size; }
        bool isEmpty() const { return _size == 0; }
        bool isNotEmpty() const { return _size > 0; }

        QList<NamedNode> namedNodes() const {
            QList<NamedNode> namedNodes;
            QSet<QString> removedKeys;
            const Dictionary *dict = this;
            while(dict != root()) {
                if(dict->_removedKeys) removedKeys.unite(*dict->_removedKeys);
                if(dict->_addedKeys) {
                    QListIterator<QString> i(*dict->_addedKeys);
                    i.toBack();
                    while(i.hasPrevious()) {
                        QString key = i.previous();
                        if(!removedKeys.contains(key)) {
                            Node *value = dict->_hash->value(key);
                            if(dict != this) {
                                value = value->fork()->setIsVirtual(true);
                                if(!_hash) { _hash = new NodeHash; }
                                _hash->insert(key, value);
                                const_cast<Dictionary *>(this)->addDirectChild(_keyToName(key), value);
                            }
                            namedNodes.prepend(NamedNode(key, value));
                        }
                    }
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            return namedNodes;
        }

        QList<QString> keys() const {
            QList<QString> keys;
            QSet<QString> removedKeys;
            const Dictionary *dict = this;
            while(dict != root()) {
                if(dict->_removedKeys) removedKeys.unite(*dict->_removedKeys);
                if(dict->_addedKeys) {
                    QListIterator<QString> i(*dict->_addedKeys);
                    i.toBack();
                    while(i.hasPrevious()) {
                        QString key = i.previous();
                        if(!removedKeys.contains(key)) keys.prepend(unescapedKey(key));
                    }
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            return keys;
        }

        NodeList values() const;

        bool hasKey(QString key) const {
            checkKey(key);
            escapeKey(key);
            return _hasKey(key);
        }

        bool _hasKey(const QString &key) const {
            if(isEmpty()) return false;
            const Dictionary *dict = this;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) return dict->_hash->value(key) != NULL;
                dict = static_cast<Dictionary *>(dict->origin());
            }
            return false;
        }

        bool hasValue(Node *value) const;

        Dictionary *remove(QString key) {
            checkKey(key);
            escapeKey(key);
            const Dictionary *dict = this;
            bool found = false;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) {
                    if(!dict->_hash->value(key)) throw RuntimeException("alreay removed key");
                    found = true;
                    break;
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            if(!found) throw NotFoundException("key not found");
            _size--;
            if(!_hash) { _hash = new NodeHash; }
            _hash->insert(key, NULL);
            if(!_removedKeys) { _removedKeys = new QSet<QString>; }
            _removedKeys->insert(key);
            if(dict == this) {
                removeDirectChild(_keyToName(key));
            }
            return this;
        }

        Dictionary *clear() {
            foreach(QString key, keys()) remove(key);
            return this;
        }

        static void escapeKey(QString &key) {
            key.replace("\\", "\\\\");
        }

        static QString unescapedKey(QString key) {
            key.replace("\\\\", "\\");
            return key;
        }

        static const bool keyIsAnonymous(const QString &key) {
            return key.at(0) == '\\' && key.size()>1 && key.at(1) != '\\';
        }

        virtual const QString inspect() const {
            QString str = "[";
            bool first = true;
            foreach(QString key, keys()) {
                if(!first) str.append(", "); else first = false;
                str.append(QString("%1: %2").arg(key).arg(const_cast<Dictionary *>(this)->get(key)->inspect()));
            }
            str.append("]");
            return str;
        }
    private:
        mutable NodeHash *_hash;
        QList<QString> *_addedKeys;
        QSet<QString> *_removedKeys;
        int _size;
        int _anonymousKeyCount;

        void checkKey(const QString &key) const {
            if(key.isEmpty()) throw ArgumentException("key is empty");
        }

        void checkValue(Node *value) const {
            if(!value) throw NullPointerException("Node pointer is NULL");
        }

        const QString _keyToName(const QString &key) const { return QString("\\[%1:]").arg(key); }
    };
}

#endif // CHILD_DICTIONARY_H
