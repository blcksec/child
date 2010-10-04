#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "child/nativemethod.h"
#include "child/object.h"

#define CHILD_DICTIONARY(EXPRESSION) static_cast<Dictionary *>(EXPRESSION)

namespace Child {
    class Dictionary : public Object {
    public:
        Dictionary() : _hash(NULL), _addedKeys(NULL), _removedKeys(NULL), _size(0), _anonymousKeyCount(0) {}

        virtual ~Dictionary() {
            delete _hash;
            delete _addedKeys;
        }

        static Dictionary *root();

        virtual Dictionary *fork() {
            Dictionary *dict = _fork(this);
            dict->_size = _size;
            dict->_anonymousKeyCount = _anonymousKeyCount;
            return(dict);
        }

        Node *get(QString key) {
            if(key.isEmpty()) { throw ArgumentException("Empty key passed to Dictionary::get()"); }
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
            if(!value) { throw NotFoundException("Key not found in Dictionary::get()"); }
            if(dict != this) {
                value = value->fork()->setIsVirtual(true);
                if(!_hash) { _hash = new NodeHash; }
                _hash->insert(key, value);
                addDirectChild(QString("\\[%1]").arg(key), value);
            }
            return(value);
        }

        Node *set(QString key, Node *value) {
            if(key.isEmpty()) {
                key = QString("\%1").arg(_anonymousKeyCount);
                _anonymousKeyCount++;
            } else {
                escapeKey(key);
            }
            if(!value) { throw NullPointerException("NULL value passed to Dictionary::set()"); }
            if(!_hasKey(key)) {
                _size++;
                if(!_addedKeys) { _addedKeys = new QList<QString>; }
                _addedKeys->append(key);
            }
            if(!_hash) { _hash = new NodeHash; }
            _hash->insert(key, value);
            addOrSetDirectChild(QString("\\[%1]").arg(key), value);
            return(value);
        }

        const QString &key(Node *value) const;
        // cherche value ou ancètre de value ?

        Node *value(const QString &key) { return(get(key)); }
        int size() const { return(_size); }
        bool isEmpty() const { return(_size == 0); }

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
                        if(!removedKeys.contains(key)) keys.prepend(key);
                    }
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            return(keys);
        }

        NodeList values() const;

        bool hasKey(QString key) const {
            if(key.isEmpty()) { throw ArgumentException("Empty key passed to Dictionary::set()"); }
            escapeKey(key);
            return(_hasKey(key));
        }

        bool _hasKey(const QString &key) const {
            if(isEmpty()) return(false);
            const Dictionary *dict = this;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) return(dict->_hash->value(key) != NULL);
                dict = static_cast<Dictionary *>(dict->origin());
            }
            return(false);
        }

        bool hasValue(Node *value) const;

        Dictionary *remove(QString key) {
            if(key.isEmpty()) { throw ArgumentException("Empty key passed to Dictionary::remove()"); }
            escapeKey(key);
            const Dictionary *dict = this;
            bool found = false;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) {
                    if(!dict->_hash->value(key)) { throw RuntimeException("Alreay removed key passed to Dictionary::remove()"); }
                    found = true;
                    break;
                }
                dict = static_cast<Dictionary *>(dict->origin());
            }
            if(!found) { throw NotFoundException("Key not found in Dictionary::remove()"); }
            _size--;
            if(!_hash) { _hash = new NodeHash; }
            _hash->insert(key, NULL);
            if(!_removedKeys) { _removedKeys = new QSet<QString>; }
            _removedKeys->insert(key);
            if(dict == this) {
                removeDirectChild(QString("\\[%1]").arg(key));
            }
            return(this);
        }

        Dictionary *clear() {
            foreach(QString key, keys()) remove(key);
            return(this);
        }

        static void escapeKey(QString &key) {
            key.replace("\\", "\\\\");
        }

        static void unescapeKey(QString &key) {
            key.replace("\\\\", "\\");
        }
    private:
        static Dictionary *_root;
        NodeHash *_hash;
        QList<QString> *_addedKeys;
        QSet<QString> *_removedKeys;
        int _size;
        int _anonymousKeyCount;
    };
}

#endif // DICTIONARY_H
