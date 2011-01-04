#ifndef CHILD_NATIVE_METHOD_H
#define CHILD_NATIVE_METHOD_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

#define CHILD_NATIVE_METHOD(ARGS...) new NativeMethod(Node::context()->child("NativeMethod"), ##ARGS)

#define CHILD_NATIVE_METHOD_ADD(CLASS, METHOD, NAME...) \
CLASS::root()->addOrSetChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_)))

#define CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(CLASS, METHOD, NAME...) \
CLASS::root()->addOrSetChild(preferSecondArgumentIfNotEmpty(#METHOD, #NAME), \
    new NativeMethod(NativeMethod::root(), static_cast<_MethodPointer_>(&CLASS::_##METHOD##_), true))

typedef Node *(Node::*_MethodPointer_)(Message *, Language::Primitive *);

class NativeMethod : public Node {
    CHILD_DECLARE(NativeMethod, Node);
public:
    explicit NativeMethod(Node *origin, const _MethodPointer_ &method = NULL, const QString &codeInputName = "") :
        Node(origin), _method(method), _codeInputName(codeInputName) {}

    explicit NativeMethod(Node *origin, const _MethodPointer_ &method, bool hasCodeInput) :
        Node(origin), _method(method), _codeInputName(hasCodeInput ? "code" : "") {}

    CHILD_FORK_METHOD(NativeMethod, method(), codeInputName());

    _MethodPointer_ method() const { return _method; }
    void setMethod(const _MethodPointer_ &method) { _method = method; }

    const QString &codeInputName() const { return _codeInputName; }
    void setCodeInputName(const QString &name) { _codeInputName = name; }
    bool hasCodeInput() const { return !_codeInputName.isEmpty(); }
    void setHasCodeInput(bool hasCodeInput) { setCodeInputName(hasCodeInput ? "code" : ""); }

    virtual Node *run(Node *receiver, Message *message, Primitive *code = NULL);
private:
    _MethodPointer_ _method;
    QString _codeInputName;
};

CHILD_END

#endif // CHILD_NATIVE_METHOD_H
