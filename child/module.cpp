#include "child/module.h"
#include "child/nativemethod.h"
#include "child/text.h"

namespace Child {
    TaggedModule::TaggedModule(const QString &t, Module *const m) : tag(t), module(m) {
        if(t.isEmpty()) { throw ArgumentException("Empty tag passed to TaggedModule constructor()"); }
        if(!m) { throw NullPointerException("NULL module passed to TaggedModule constructor()"); }
    }

    long long int Module::_moduleCount = 0;
    Module *Module::_root = Module::root();

    Module::~Module() {
        foreach(Module *mod, _modules) {
            removeModule(mod);
        }
        foreach(Module *fork, _forks) {
            delete fork;
        }
        foreach(TaggedModule value, _parents) {
            removeParent(value.tag, value.module, false);
        }
        foreach(QString tag, _children.keys()) {
            removeDirectChild(tag);
        }
        _moduleCount--;
    }

    Module *Module::root() {
        if(!_root) {
            _root = new Module;
            _root->addParent("Module", _root);
        }
        return(_root);
    }

    bool Module::hasDirectModule(Module *mod) const {
        if(!mod) { throw NullPointerException("NULL value passed to hasDirectModule()"); }
        return(_modules.contains(mod));
    }

    void Module::addModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to addModule()"); }
        if(_modules.contains(mod)) { throw DuplicateException("Duplicate module passed to addModule()"); }
        _modules.append(mod);
        mod->_forks.append(this);
    }

    void Module::prependModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to prependModule()"); }
        if(_modules.contains(mod)) { throw DuplicateException("Duplicate module passed to prependModule()"); }
        _modules.prepend(mod);
        mod->_forks.append(this);
    }

    void Module::removeModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to removeModule()"); }
        if(!_modules.contains(mod)) { throw NotFoundException("Missing module passed to removeModule()"); }
        _modules.removeOne(mod);
        mod->_forks.removeOne(this);
    }

    const ModuleMultiHash Module::parents() const {
        ModuleMultiHash hash;
        foreach(TaggedModule value, _parents) {
            hash.insert(value.tag, value.module);
        }
        return(hash);
    }

    bool Module::hasDirectParent(Module *mod) const {
        if(!mod) { throw NullPointerException("NULL value passed to hasDirectParent()"); }
        return(!mod->_children.key(const_cast<Module *>(this)).isEmpty());
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

    bool Module::hasDirectChild(const QString &tag) const {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to hasDirectChild()"); }
        return(_children.contains(tag));
    }

    Module *Module::directChild(const QString &tag) const {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to directChild()"); }
        Module *value = _children.value(tag);
        if(!value) { throw NotFoundException("Missing tag passed to directChild()"); }
        return(value);
    }

    Module *Module::addDirectChild(const QString &tag, Module *mod) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to addDirectChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to addDirectChild()"); }
        if(_children.contains(tag)) { throw DuplicateException("Duplicate tag passed to addDirectChild()"); }
        mod->addParent(tag, this);
        return(mod);
    }

    Module *Module::setDirectChild(const QString &tag, Module *mod) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to setDirectChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to setDirectChild()"); }
        if(!_children.contains(tag)) { throw NotFoundException("Missing tag passed to setDirectChild()"); }
        Module *currentChild = directChild(tag);
        if(currentChild != mod) {
            currentChild->removeParent(tag, this);
            mod->addParent(tag, this);
        }
        return(mod);
    }

    void Module::removeDirectChild(const QString &tag) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to removeDirectChild()"); }
        Module *mod = _children.value(tag);
        if(!mod) { throw NotFoundException("Missing tag passed to removeDirectChild()"); }
        mod->removeParent(tag, this);
    }

    Module *Module::_getOrSetChild(const QString &tag, Module *setValue, bool returnThisIfFound) {
        ModuleSet moduleSeen;
        ModuleQueue moduleQueue;
        ModuleQueue parentQueue;
        parentQueue.enqueue(this);
        QHash<Module *, TaggedModule> parentTree;
        Module *child = NULL;
        while (!parentQueue.isEmpty()) {
            Module *parent = parentQueue.dequeue();
            moduleQueue.enqueue(parent);
            while (!moduleQueue.isEmpty()) {
                Module *module = moduleQueue.dequeue();
//                p(module->inspect());
                if(module->hasDirectChild(tag)) { child = module->directChild(tag); break; }
                foreach(Module *mod, module->_modules) {
                    if(!moduleSeen.contains(mod)) {
                        moduleSeen.insert(mod);
                        moduleQueue.enqueue(mod);
                    }
                }
                foreach(TaggedModule par, module->_parents) {
                    if(!parentTree.contains(par.module)) {
                        parentTree.insert(par.module, TaggedModule(par.tag, parent));
                        parentQueue.enqueue(par.module);
                    }
                }
            }
            if(child) {
                if(returnThisIfFound) { return(this); }
                TaggedModuleList parentPath;
                while(parent != this) {
                    TaggedModule par = parentTree.value(parent);
                    parentPath.prepend(TaggedModule(par.tag, parent));
                    parent = par.module;
                }
                bool mustVirtualize = false;
                foreach(TaggedModule par, parentPath) {
                    if(!mustVirtualize && !parent->_parents.contains(par)) mustVirtualize = true;
                    if(mustVirtualize) {
//                        p("Forking parent " + par.module->inspect());
                        Module *virtualParent = par.module->fork()->setIsVirtual(true);
                        parent->addParent(par.tag, virtualParent);
                        parent = virtualParent;
                    } else {
                        parent = par.module;
                    }
                }
                if(mustVirtualize || !child->_parents.contains(TaggedModule(tag, parent))) {
                    if(setValue) {
                        child = setValue;
                    } else {
                        // p("Cloning child " + child->inspect());
                        child = child->fork()->setIsVirtual(true);
                    }
                    child->addParent(tag, parent);
                } else {
                    if(setValue) {
                        if(child != setValue) {
                            child->removeParent(tag, parent);
                            child = setValue;
                            child->addParent(tag, parent);
                        }
                    }
                }
                NativeMethod *meth = dynamic_cast<NativeMethod *>(child);
                if(meth && meth->method()) {
                    child = (parent->*meth->method())();
                }
                return(child);
            }
        }
        return(NULL);
    }

    bool Module::hasChild(const QString &tag) const {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to hasChild()"); }
        return(const_cast<Module *>(this)->_getOrSetChild(tag, NULL, true));
    }

    Module *Module::child(const QString &tag) const {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to child()"); }
        Module *child = const_cast<Module *>(this)->_getOrSetChild(tag);
        if(!child) { throw NotFoundException("Child not found in child()"); }
        return(child);
    }

    Module *Module::setChild(const QString &tag, Module *mod) {
        if(tag.isEmpty()) { throw ArgumentException("Empty tag passed to setChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to setChild()"); }
        Module *child = _getOrSetChild(tag, mod);
        if(!child) { throw NotFoundException("Child not found in setChild()"); }
        return(child);
    }

    const QString Module::inspect() const {
        QString str;
        str = uniqueHexID();
        str.append(": [");
        str.append(QStringList(parents().keys()).join(", "));
        str.append("] => [");
        str.append(QStringList(children().keys()).join(", "));
        str.append("]");
        return str;
    }

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
