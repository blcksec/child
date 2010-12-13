#ifndef CHILD_POINTER_H
#define CHILD_POINTER_H

#include "child/global.h"

CHILD_BEGIN

#define CHILD_POINTER_CHECK_DATA \
if(!data()) CHILD_THROW_NULL_POINTER_EXCEPTION("NULL pointer dereferenced")

#define CHILD_POINTER_CHECK_CAST(NAME, DATA) \
if((DATA) && !dynamic_cast<const NAME *>(DATA)) CHILD_THROW_TYPECAST_EXCEPTION("typecasting failed")

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

class Reference : public Pointer { // Can be used as key in hash (comparisons by value)
public:
    Reference() : Pointer() {}
    Reference(const Pointer &other) : Pointer(other) {}
};

#define CHILD_POINTER_DECLARATION(NAME, ORIGIN) \
class NAME; \
class NAME##Pointer : public ORIGIN##Pointer { \
public: \
    NAME##Pointer() {} \
    explicit NAME##Pointer(const NAME &other); \
    explicit NAME##Pointer(const Node &other, bool dynamicCast = false); \
    NAME##Pointer(const NAME *other); \
    NAME##Pointer(const NAME##Pointer &other); \
    NAME##Pointer(const Pointer &other, bool dynamicCast = false); \
    static NAME##Pointer dynamicCast(const Pointer &other); \
    \
    NAME &operator*(); \
    const NAME &operator*() const; \
    NAME *operator->(); \
    const NAME *operator->() const; \
    NAME##Pointer &operator=(const NAME##Pointer &other); \
    static const NAME##Pointer &null() { static const NAME##Pointer _null; return _null; }; \
};

#define CHILD_POINTER_DEFINITION(NAME, ORIGIN) \
inline NAME##Pointer::NAME##Pointer(const NAME &other) { initData(&other); } \
inline NAME##Pointer::NAME##Pointer(const Node &other, bool dynamicCast) { \
    if(dynamicCast) \
        initData(dynamic_cast<const NAME *>(&other)); \
    else { \
        CHILD_POINTER_CHECK_CAST(NAME, &other); \
        initData(&other); \
    } \
} \
inline NAME##Pointer::NAME##Pointer(const NAME *other) { CHILD_POINTER_CHECK_CAST(NAME, other); initData(other); } \
inline NAME##Pointer::NAME##Pointer(const NAME##Pointer &other) : ORIGIN##Pointer(other) {} \
inline NAME##Pointer::NAME##Pointer(const Pointer &other, bool dynamicCast) { \
    if(dynamicCast) \
        initData(dynamic_cast<const NAME *>(other.data())); \
    else { \
        CHILD_POINTER_CHECK_CAST(NAME, other.data()); \
        initData(other.data()); \
    } \
} \
inline NAME##Pointer NAME##Pointer::dynamicCast(const Pointer &other) { return dynamic_cast<const NAME *>(other.data()); } \
\
inline NAME &NAME##Pointer::operator*() { CHILD_POINTER_CHECK_DATA; return *static_cast<NAME *>(data()); }; \
inline const NAME &NAME##Pointer::operator*() const { CHILD_POINTER_CHECK_DATA; return *static_cast<const NAME *>(data()); }; \
inline NAME *NAME##Pointer::operator->() { CHILD_POINTER_CHECK_DATA; return static_cast<NAME *>(data()); }; \
inline const NAME *NAME##Pointer::operator->() const { CHILD_POINTER_CHECK_DATA; return static_cast<const NAME *>(data()); }; \
inline NAME##Pointer &NAME##Pointer::operator=(const NAME##Pointer &other) { CHILD_POINTER_CHECK_CAST(NAME, other.data()); setData(other.data()); return *this; }

CHILD_END

#endif // CHILD_POINTER_H
