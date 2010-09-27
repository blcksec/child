#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "child/nativemethod.h"
#include "child/object.h"

#define CHILD_DICTIONARY(EXPRESSION) static_cast<Dictionary *>(EXPRESSION)

namespace Child {
    class Dictionary : public Object {
    public:
        Dictionary() : _hash(NULL), _keys(NULL), _size(0), _anonymousKeyCount(0) {}

        virtual ~Dictionary() {
            delete _hash;
            delete _keys;
        }

        static Dictionary *root();

        virtual Dictionary *fork() {
            Dictionary *dict = _fork(this);
            dict->_size = _size;
            dict->_anonymousKeyCount = _anonymousKeyCount;
            return(dict);
        }

        Module *get(QString key) const {
            if(key.isEmpty()) { throw ArgumentException("Empty key passed to Dictionary::get()"); }
            escapeKey(key);
            const Dictionary *dict = this;
            Module *value = NULL;
            while(dict != root()) {
                if(dict->_hash && dict->_hash->contains(key)) {
                    value = dict->_hash->value(key);
                    break;
                }
                dict = static_cast<Dictionary *>(dict->baseModule());
            }
            if(!value) { throw NotFoundException("Key not found in Dictionary::get()"); }
            return(value);
        }

        Module *set(QString key, Module *value) {
            if(key.isEmpty()) {
                key = QString("\%1").arg(_anonymousKeyCount);
                _anonymousKeyCount++;
            } else {
                escapeKey(key);
            }
            if(!value) { throw NullPointerException("NULL value passed to Dictionary::set()"); }
            if(!_hasKey(key)) {
                _size++;
                if(!_keys) { _keys = new QList<QString>; }
                _keys->append(key);
            }
            if(!_hash) { _hash = new ModuleHash; }
            _hash->insert(key, value);
            addOrSetDirectChild(QString("\\[%1]").arg(key), value);
            return(value);
        }

        const QString &key(Module *value) const;
        Module *value(const QString &key) const { return(get(key)); }
        int size() const { return(_size); }
        bool isEmpty() const { return(_size == 0); }
        QList<QString> keys() const;
        ModuleList values() const;

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
                dict = static_cast<Dictionary *>(dict->baseModule());
            }
            return(false);
        }

        bool hasValue(Module *value) const;
        Module *remove(const QString &key);
        Dictionary *clear();

        static void escapeKey(QString &key) {
            key.replace("\\", "\\\\");
        }

        static void unescapeKey(QString &key) {
            key.replace("\\\\", "\\");
        }
    private:
        static Dictionary *_root;
        ModuleHash *_hash;
        QList<QString> *_keys;
        int _size;
        int _anonymousKeyCount;
    };
}

#endif // DICTIONARY_H
