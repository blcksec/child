#include "child/module.h"
#include "child/nativemethod.h"
#include "child/text.h"

namespace Child {
    TaggedModule::TaggedModule(const QString &t, Module *const m) : tag(t), module(m) {
        if(t.isEmpty()) { throw ArgumentException("Empty tag passed to TaggedModule constructor()"); }
        if(!m) { throw NullPointerException("NULL module passed to TaggedModule constructor()"); }
    }

    long long int Module::_moduleCount = 0;

    Module::~Module() {
        foreach(Module *mod, _modules) {
            removeModule(mod);
        }
        foreach(Module *clone, _clones) {
            delete clone;
        }
        foreach(TaggedModule value, _parents) {
            removeParent(value.tag, value.module, false);
        }
        foreach(QString tag, _children.keys()) {
            removeChild(tag);
        }
        _moduleCount--;
    }

    void Module::addModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to addModule()"); }
        if(_modules.contains(mod)) { throw DuplicateException("Duplicate module passed to addModule()"); }
        _modules.append(mod);
        mod->_clones.append(this);
    }

    void Module::prependModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to prependModule()"); }
        if(_modules.contains(mod)) { throw DuplicateException("Duplicate module passed to prependModule()"); }
        _modules.prepend(mod);
        mod->_clones.append(this);
    }

    void Module::removeModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to removeModule()"); }
        if(!_modules.contains(mod)) { throw NotFoundException("Missing module passed to removeModule()"); }
        _modules.removeOne(mod);
        mod->_clones.removeOne(this);
    }

    Module *Module::clone() const {
        Module *mod = new Module;
        mod->addModule(const_cast<Module *>(this));
        return(mod);
    }

    const ModuleMultiHash Module::parents() const {
        ModuleMultiHash hash;
        foreach(TaggedModule value, _parents) {
            hash.insert(value.tag, value.module);
        }
        return(hash);
    }

    void Module::addParent(const QString &tag, Module *mod) {
        TaggedModule taggedModule = TaggedModule(tag, mod);
        if(_parents.contains(taggedModule)) { throw DuplicateException("Duplicate module/tag passed to addParent()"); }
        if(mod->_children.contains(tag)) { throw DuplicateException("Duplicate tag in parent passed to addParent()"); }
        _parents.insert(taggedModule);
        mod->_children.insert(tag, this);
    }

    void Module::removeParent(const QString &tag, Module *mod, bool callDeleteIfOrphan) {
        TaggedModule taggedModule = TaggedModule(tag, mod);
        if(!_parents.contains(taggedModule)) { throw NotFoundException("Missing module/tag passed to removeParent()"); }
        _parents.remove(taggedModule);
        mod->_children.remove(tag);
        if(callDeleteIfOrphan) deleteIfOrphan();
    }

    Module *Module::child(const QString &tag) const {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to child()"); }
        return(_children.value(tag));
    }

    void Module::setChild(const QString &tag, Module *mod) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to setChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to setChild()"); }
        if(Module *currentChild = child(tag)) {
            if(currentChild == mod) return;
            currentChild->removeParent(tag, this);
        }
        mod->addParent(tag, this);
    }

    void Module::removeChild(const QString &tag) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to removeChild()"); }
        Module *mod = _children.value(tag);
        if(!mod) { throw NotFoundException("Missing tag passed to removeChild()"); }
        mod->removeParent(tag, this);
    }

//    Module *const Module::findParentInSelfOrGenres() const {
//        if(Module *par = parent()) { return(par); }
//        if(Module *gnr = genre()) {
//            return(gnr->findParentInSelfOrGenres());
//        } else {
//            return(NULL);
//        }
//    }

//    bool Module::findSlot(const QString &name, Module *&own, Module *&rcv, Module *&val) const {
//        ModuleSet objSeen;
//        return(findSlotInSelfAndParents(name, own, rcv, val, objSeen));
//    }

//    bool Module::findSlotInSelfAndParents(const QString &name, Module *&own, Module *&rcv, Module *&val, ModuleSet &objSeen) const {
//        if(findSlotInSelfMixinsAndGenres(name, own, val, objSeen)) {
//            rcv = const_cast<Module *>(this);
//            return(true);
//        }
//        Module *par = findParentInSelfOrGenres();
//        if(!par) { return(false); }
//        return(par->findSlotInSelfAndParents(name, own, rcv, val, objSeen));
//    }

//    bool Module::findSlotInSelfMixinsAndGenres(const QString &name, Module *&own, Module *&val, ModuleSet &objSeen) const {
//        if(objSeen.contains(const_cast<Module *>(this))) { return(false); }
//        objSeen << const_cast<Module *>(this);
//        if(hasSlot(name, val)) { own = const_cast<Module *>(this); return(true); }
//        // FIXME: mixins search here
//        if(!genre()) { return(false); }
//        return(genre()->findSlotInSelfMixinsAndGenres(name, own, val, objSeen));
//    }

//    Module *const Module::send(const QString &name) {
//        Module *own = NULL, *rcv = NULL, *val = NULL;
//        if(!findSlot(name, own, rcv, val)) { return(NULL); }
//        NativeMethod *meth = dynamic_cast<NativeMethod *>(val);
//        if(meth && meth->method()) {
//            return((rcv->*meth->method())());
//        }
//        return(val);
//    }

//    Module *const Module::fatalSend(const QString &name) {
//        Module *obj = send(name);
//        if(!obj) { qFatal("Object '%s' not found", qPrintable(name)); }
//        return(obj);
//    }

//    Module *Module::toText() {
//        QString str;
//        if(genre()) {
//            str.append(genre()->identityOrAddress());
//            str.append(":");
//        }
//        str.append(identityOrAddress());

//        str.append(" {");
//        bool first = true;
//        ModuleHashIterator i(slots_());
//        while(i.hasNext()) {
//            i.next();
//            if(!first) { str.append(", "); } else { first = false; }
//            str.append(i.key());
//            str.append(": ");
//            str.append(i.value()->identityOrAddress());
//        }
//        str.append("}");

//        return(Text::create(this, str));
//    }

//    Module *Module::print() {
//        p(dynamic_cast<Text *>(toText())->value());
//        return NULL;
//    }
}
