#ifndef CHILD_LIST_H
#define CHILD_LIST_H

#include "child/object.h"

namespace Child {
    class List : public Object {
        CHILD_DECLARATION(List);
    public:
        List() : _list(NULL), _uniqueNumber(0) {}

        virtual ~List() {
            delete _list;
        }

        virtual void initFork() {
            List *orig = List::as(origin());
            if(orig->_list) _list = new NumberedNodeList(*orig->_list); // TODO: optimized fork!
            _uniqueNumber = orig->_uniqueNumber;
        }

        Node *insert(int i, Node *value) {
            checkIndex(i, true);
            checkValue(value);
            if(!_list) { _list = new NumberedNodeList; }
            _list->insert(i, NumberedNode(_uniqueNumber, value));
            addDirectChild(_numberToName(_uniqueNumber), value);
            _uniqueNumber++;
            return(value);
        }

        List *insert(int i, List *otherList) {
            checkIndex(i, true);
            if(!otherList) throw(NullPointerException("List pointer is NULL"));
            for(int j = 0; j < otherList->size(); j++) {
                insert(i + j, otherList->get(j));
            }
            return(otherList);
        }

        Node *append(Node *value) { return(insert(size(), value)); }
        List *append(List *otherList) { return(insert(size(), otherList)); }
        Node *prepend(Node *value) { return(insert(0, value)); }
        List *prepend(List *otherList) { return(insert(0, otherList)); }

        Node *get(int i) {
            checkIndex(i);
            return(_list->at(i).node);
        }

        Node *set(int i, Node *value) {
            checkIndex(i);
            checkValue(value);
            int number = _list->at(i).number;
            _list->replace(i, NumberedNode(number, value));
            setDirectChild(_numberToName(number), value);
            return(value);
        }

        void remove(int i) {
            checkIndex(i);
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

        void checkIndex(int i, bool insertMode = false) const {
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

        class const_iterator {
        public:
            List *list;
            int i;
            const_iterator() : list(NULL), i(0) {}
            const Node *operator*() const { return(list->get(i)); }
            const_iterator &operator++() { ++i; return(*this); }
            bool operator!=(const const_iterator &o) const { return i != o.i; }
        };

        const_iterator begin() const {
            List::const_iterator i;
            i.list = const_cast<List *>(this);
            return i;
        }

        const_iterator end() const {
            List::const_iterator i;
            i.list = const_cast<List *>(this);
            i.i = size();
            return i;
        }
    private:
        NumberedNodeList *_list;
        int _uniqueNumber;

        void checkValue(Node *value) const {
            if(!value) throw(NullPointerException("Node pointer is NULL"));
        }

        const QString _numberToName(int number) const { return(QString("\\[%1]").arg(number)); }
    };
}

#endif // CHILD_LIST_H
