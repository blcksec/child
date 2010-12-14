#ifndef CHILD_ENVELOPE_H
#define CHILD_ENVELOPE_H

#include "child/language/primitive.h"

CHILD_BEGIN

using namespace Language;

CHILD_POINTER_DECLARATION(Envelope, Primitive);

#define CHILD_ENVELOPE(ARGS...) EnvelopePointer(new Envelope(Node::context()->child("Object", "Envelope"), ##ARGS))

class Envelope : public Primitive {
    CHILD_DECLARATION(Envelope, Primitive);
public:
    Envelope(const Pointer &origin, const Pointer &contents = NULL, const QStringRef &sourceCodeRef = QStringRef(),
             const Pointer &sender = NULL, const Pointer &receiver = NULL) :
        Primitive(origin, contents, sourceCodeRef), _sender(sender), _receiver(receiver) {}

    Envelope(const Pointer &origin, const PrimitivePointer &primitive,
             const Pointer &sender = NULL, const Pointer &receiver = NULL) :
        Primitive(origin, primitive->value(), primitive->sourceCodeRef()), _sender(sender), _receiver(receiver) {}

    static void initRoot() { Object::root()->addChild("Envelope", root()); }
    virtual Pointer fork() const {
        return new Envelope(this, forkIfNotNull(contents()), sourceCodeRef(),
                            forkIfNotNull(sender()), forkIfNotNull(receiver()));
    }

    Pointer contents() const { return value(); } // aliases
    void setContents(const Pointer &contents) { setValue(contents); }

    Pointer sender() const { return _sender; }
    void setSender(const Pointer &sender) { _sender = sender; }

    Pointer receiver() const { return _sender; }
    void setReceiver(const Pointer &receiver) { _receiver = receiver; }
private:
    Pointer _sender;
    Pointer _receiver;
};

CHILD_POINTER_DEFINITION(Envelope, Primitive);

CHILD_END

#endif // CHILD_ENVELOPE_H
