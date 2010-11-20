#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "child/object.h"

#define CHILD_LIST(EXPRESSION) static_cast<List *>(EXPRESSION)

namespace Child {
    class List : public Object {
    public:
        static List *root();
        static List *fork(Node *world) { return(CHILD_LIST(world->child("List"))->fork()); }

        List() : _list(NULL), _uniqueNumber(0) {}

        virtual ~List() {
            delete _list;
        }

        virtual List *fork() {
            List *list = _fork(this);
            if(_list) list->_list = new NumberedNodeList(*_list); // TODO: optimized fork!
            list->_uniqueNumber = _uniqueNumber;
            return(list);
        }

        Node *insert(int i, Node *value) {
            _checkIndex(i, true);
            _checkValue(value);
            if(!_list) { _list = new NumberedNodeList; }
            _list->insert(i, NumberedNode(_uniqueNumber, value));
            addDirectChild(_numberToName(_uniqueNumber), value);
            _uniqueNumber++;
            return(value);
        }

        Node *append(Node *value) { return(insert(size(), value)); }
        Node *prepend(Node *value) { return(insert(0, value)); }

        Node *get(int i) {
            _checkIndex(i);
            return(_list->at(i).node);
        }

        Node *set(int i, Node *value) {
            _checkIndex(i);
            _checkValue(value);
            int number = _list->at(i).number;
            _list->replace(i, NumberedNode(number, value));
            setDirectChild(_numberToName(number), value);
            return(value);
        }

        void remove(int i) {
            _checkIndex(i);
            int number = _list->at(i).number;
            _list->removeAt(i);
            removeDirectChild(_numberToName(number));
        }

        List *clear() {
            if(_list) {
                foreach(NumberedNode numNode, *_list)
                    removeDirectChild(_numberToName(numNode.number));
                _list->clear();
            }
            return(this);
        }

        int size() const { return(_list ? _list->size() : 0); }
        bool isEmpty() const { return(size() == 0); }
        bool isNotEmpty() const { return(size() > 0); }

        virtual const QString inspect() const {
            QString str = "[";
            bool first = true;
            for(int i = 0; i < size(); i++) {
                if(!first) str.append(", "); else first = false;
                str.append(const_cast<List *>(this)->get(i)->inspect());
            }
            str.append("]");
            return(str);
        }
    private:
        static List *_root;
        NumberedNodeList *_list;
        int _uniqueNumber;

        void _checkIndex(int i, bool insertMode = false) const {
            int max = size();
            if(!insertMode) max--;
            if(i < 0) throw(IndexOutOfBoundsException("index is less than zero"));
            if(i > max) {
                if(insertMode)
                    throw(IndexOutOfBoundsException("index is greater than size"));
                else
                    throw(IndexOutOfBoundsException("index is greater than size-1"));
            }
        }

        void _checkValue(Node *value) const {
            if(!value) throw(NullPointerException("value pointer is NULL"));
        }

        const QString _numberToName(int number) const { return(QString("\\[%1]").arg(number)); }
    };
}

#endif // CHILD_LIST_H
