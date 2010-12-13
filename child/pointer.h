#ifndef CHILD_POINTER_H
#define CHILD_POINTER_H

#include "child/global.h"

CHILD_BEGIN

#define CHILD_POINTER_CHECK_DATA \
if(!data()) CHILD_THROW_NULL_POINTER_EXCEPTION("NULL pointer dereferenced")

template<typename T>
class GenericPointer {
public:
    GenericPointer() : _data(NULL) {}
    explicit GenericPointer(const T &other) { initData(&other); }
    GenericPointer(const T *other) { initData(other); }
    GenericPointer(const GenericPointer &other) { initData(other.data()); }
    virtual ~GenericPointer() { if(data()) data()->release(); }

    T *data() { return const_cast<T *>(_data); }
    const T *data() const { return _data; }
    void initData(const T *data) {
        if(data) data->retain();
        _data = data;
    }
    void setData(const T *data) {
        if(_data != data) {
            if(_data) _data->release();
            if(data) data->retain();
            _data = data;
        }
    }
    void clear() { setData(NULL); }

    T &operator*() { CHILD_POINTER_CHECK_DATA; return *data(); }
    const T &operator*() const { CHILD_POINTER_CHECK_DATA; return *data(); }
    T *operator->() { CHILD_POINTER_CHECK_DATA; return data(); }
    const T *operator->() const { CHILD_POINTER_CHECK_DATA; return data(); }
    GenericPointer &operator=(const GenericPointer &other) { setData(other.data()); return *this; }

    bool isNull() const { return !data(); }
    bool isNotNull() const { return data(); }
    operator bool() const { return !isNull(); }
    bool operator!() const { return isNull(); }

    static const GenericPointer &null() { static const GenericPointer _null; return _null; };
private:
    const T *_data;
};

class Node;
typedef GenericPointer<Node> Pointer;
//inline bool operator==(const GenericPointer &a, const GenericPointer &b) { return a.data() == b.data(); }
//inline bool operator!=(const GenericPointer &a, const GenericPointer &b) { return a.data() != b.data(); }
//inline uint qHash(const GenericPointer &node) { return ::qHash(node.data()); }

CHILD_END

#endif // CHILD_POINTER_H
