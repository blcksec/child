#include "child/module.h"
#include "child/nativemethod.h"
#include "child/text.h"

namespace Child {
    NamedModule::NamedModule(const QString &t, Module *const m) : name(t), module(m) {
        if(t.isEmpty()) { throw ArgumentException("Empty name passed to NamedModule constructor()"); }
        if(!m) { throw NullPointerException("NULL module passed to NamedModule constructor()"); }
    }

    long long int Module::_moduleCount = 0;
    Module *Module::_root = Module::root();

    Module::~Module() {
        unsetBaseModule();
        removeAllForks();
        removeAllExtensions();
        removeAllExtendedModules();
        removeAllParents();
        removeAllDirectChildren();
        _moduleCount--;
    }

    Module *Module::root() {
        if(!_root) {
            _root = new Module;
            _root->addParent("Module", _root);
        }
        return(_root);
    }

    void Module::setBaseModule(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to Module::setBase()"); }
        if(_baseModule == mod) { return; }
        if(_baseModule) { _baseModule->_forks->removeOne(this); }
        _baseModule = mod;
        if(!mod->_forks) { mod->_forks = new ModuleList; }
        mod->_forks->append(this);
    }

    bool Module::isBasedOn(Module *mod) const {
        if(!mod) { throw NullPointerException("NULL value passed to Module::isForkedFrom()"); }
        return(_baseModule == mod);
    }

    void Module::removeAllForks() {
        if(!_forks) { return; }
        foreach(Module *mod, *_forks) { // FIXME: Not sure the following delete can break the iterator
            delete mod;
        }
        delete _forks;
        _forks = NULL;
    }

    bool Module::hasExtension(Module *mod) const {
        if(!mod) { throw NullPointerException("NULL value passed to Module::hasExtension()"); }
        return(_extensions ? _extensions->contains(mod) : false);
    }

    void Module::addExtension(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to Module::addExtension()"); }
        if(!_extensions) { _extensions = new ModuleList; }
        if(_extensions->contains(mod)) { throw DuplicateException("Duplicate module passed to Module::addExtension()"); }
        _extensions->append(mod);
        if(!mod->_extendedModules) { mod->_extendedModules = new ModuleList; }
        mod->_extendedModules->append(this);
    }

    void Module::prependExtension(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to Module::prependExtension()"); }
        if(!_extensions) { _extensions = new ModuleList; }
        if(_extensions->contains(mod)) { throw DuplicateException("Duplicate module passed to Module::prependExtension()"); }
        _extensions->prepend(mod);
        if(!mod->_extendedModules) { mod->_extendedModules = new ModuleList; }
        mod->_extendedModules->append(this);
    }

    void Module::removeExtension(Module *mod) {
        if(!mod) { throw NullPointerException("NULL value passed to Module::removeExtension()"); }
        if(!_extensions || !_extensions->contains(mod)) { throw NotFoundException("Missing module passed to Module::removeExtension()"); }
        _extensions->removeOne(mod);
        mod->_extendedModules->removeOne(this);
    }

    void Module::removeAllExtensions() {
        if(!_extensions) { return; }
        foreach(Module *mod, *_extensions) {
            mod->_extendedModules->removeOne(this);
        }
        delete _extensions;
        _extensions = NULL;
    }

    void Module::removeAllExtendedModules() {
        if(!_extendedModules) { return; }
        foreach(Module *mod, *_extendedModules) { // FIXME: Not sure the following delete can break the iterator
            delete mod;
        }
        delete _extendedModules;
        _extendedModules = NULL;
    }

    const ModuleMultiHash Module::parents() const {
        ModuleMultiHash hash;
        if(_parents) {
            foreach(NamedModule parent, *_parents) {
                hash.insert(parent.name, parent.module);
            }
        }
        return(hash);
    }

    bool Module::hasDirectParent(Module *mod) const {
        if(!mod) { throw NullPointerException("NULL value passed to Module::hasDirectParent()"); }
        return(mod->_children && !mod->_children->key(const_cast<Module *>(this)).isEmpty());
    }

    void Module::addParent(const QString &name, Module *mod) {
        NamedModule namedModule = NamedModule(name, mod);
        if(!_parents) { _parents = new NamedModuleSet; }
        if(_parents->contains(namedModule)) { throw DuplicateException("Duplicate module/name passed to Module::addParent()"); }
        if(!mod->_children) { mod->_children = new ModuleHash; }
        if(mod->_children->contains(name)) { throw DuplicateException("Duplicate name in parent passed to Module::addParent()"); }
        _parents->insert(namedModule);
        mod->_children->insert(name, this);
    }

    void Module::removeParent(const QString &name, Module *mod) {
        NamedModule namedModule = NamedModule(name, mod);
        if(!_parents || !_parents->contains(namedModule)) { throw NotFoundException("Missing module/name passed to Module::removeParent()"); }
        _parents->remove(namedModule);
        mod->_children->remove(name);
        deleteIfOrphan();
    }

    void Module::removeAllParents() {
        if(!_parents) { return; }
        foreach(NamedModule parent, *_parents) {
            parent.module->_children->remove(parent.name);
        }
        delete _parents;
        _parents = NULL;
    }

    bool Module::hasDirectChild(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::hasDirectChild()"); }
        return(_children && _children->contains(name));
    }

    Module *Module::directChild(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::directChild()"); }
        if(!_children) { throw NotFoundException("Missing name passed to Module::directChild()"); }
        Module *value = _children->value(name);
        if(!value) { throw NotFoundException("Missing name passed to Module::directChild()"); }
        return(value);
    }

    Module *Module::addDirectChild(const QString &name, Module *mod) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::addDirectChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to Module::addDirectChild()"); }
        if(_children && _children->contains(name)) { throw DuplicateException("Duplicate name passed to Module::addDirectChild()"); }
        mod->addParent(name, this);
        return(mod);
    }

    Module *Module::setDirectChild(const QString &name, Module *mod) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::setDirectChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to Module::setDirectChild()"); }
        if(!_children || !_children->contains(name)) { throw NotFoundException("Missing name passed to Module::setDirectChild()"); }
        Module *currentChild = directChild(name);
        if(currentChild != mod) {
            currentChild->removeParent(name, this);
            mod->addParent(name, this);
        }
        return(mod);
    }

    Module *Module::addOrSetDirectChild(const QString &name, Module *mod) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::addOrSetDirectChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to Module::addOrSetDirectChild()"); }
        if(!_children || !_children->contains(name)) {
            mod->addParent(name, this);
        } else {
            Module *currentChild = directChild(name);
            if(currentChild != mod) {
                currentChild->removeParent(name, this);
                mod->addParent(name, this);
            }
        }
        return(mod);
    }

    void Module::removeDirectChild(const QString &name) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::removeDirectChild()"); }
        if(!_children) { throw NotFoundException("Missing name passed to Module::removeDirectChild()"); }
        Module *mod = _children->value(name);
        if(!mod) { throw NotFoundException("Missing name passed to Module::removeDirectChild()"); }
        mod->removeParent(name, this);
    }

    void Module::removeAllDirectChildren() {
        if(!_children) { return; }
        ModuleHashIterator i(*_children);
        while(i.hasNext()) {
            i.next();
            i.value()->_parents->remove(NamedModule(i.key(), this));
            i.value()->deleteIfOrphan();
        }
        delete _children;
        _children = NULL;
    }

    Module *Module::_getOrSetChild(const QString &name, Module *setValue, bool returnThisIfFound) {
        ModuleSet moduleSeen;
        ModuleQueue moduleQueue;
        ModuleQueue parentQueue;
        parentQueue.enqueue(this);
        QHash<Module *, NamedModule> parentTree;
        Module *child = NULL;
        while(!parentQueue.isEmpty()) {
            Module *parent = parentQueue.dequeue();
            moduleQueue.enqueue(parent);
            while(!moduleQueue.isEmpty()) {
                Module *module = moduleQueue.dequeue();
//                p(module->inspect());
                if(module->hasDirectChild(name)) { child = module->directChild(name); break; }
                if(module->_baseModule) {
                    if(!moduleSeen.contains(module->_baseModule)) {
                        moduleSeen.insert(module->_baseModule);
                        moduleQueue.enqueue(module->_baseModule);
                    }
                }
                if(module->_extensions) {
                    foreach(Module *mod, *(module->_extensions)) {
                        if(!moduleSeen.contains(mod)) {
                            moduleSeen.insert(mod);
                            moduleQueue.enqueue(mod);
                        }
                    }
                }
                if(module->_parents) {
                    foreach(NamedModule par, *(module->_parents)) {
                        if(!parentTree.contains(par.module)) {
                            parentTree.insert(par.module, NamedModule(par.name, parent));
                            parentQueue.enqueue(par.module);
                        }
                    }
                }
            }
            if(child) {
                if(returnThisIfFound) { return(this); }
                NamedModuleList parentPath;
                while(parent != this) {
                    NamedModule par = parentTree.value(parent);
                    parentPath.prepend(NamedModule(par.name, parent));
                    parent = par.module;
                }
                bool mustVirtualize = false;
                foreach(NamedModule par, parentPath) {
                    if(!mustVirtualize && (!parent->_parents || !parent->_parents->contains(par))) mustVirtualize = true;
                    if(mustVirtualize) {
//                        p("Forking parent " + par.module->inspect());
                        Module *virtualParent = par.module->fork()->setIsVirtual(true);
                        parent->addParent(par.name, virtualParent);
                        parent = virtualParent;
                    } else {
                        parent = par.module;
                    }
                }
                if(mustVirtualize || !child->_parents || !child->_parents->contains(NamedModule(name, parent))) {
                    if(setValue) {
                        child = setValue;
                    } else {
                        // p("Cloning child " + child->inspect());
                        child = child->fork()->setIsVirtual(true);
                    }
                    child->addParent(name, parent);
                } else {
                    if(setValue) {
                        if(child != setValue) {
                            child->removeParent(name, parent);
                            child = setValue;
                            child->addParent(name, parent);
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

    bool Module::hasChild(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::hasChild()"); }
        return(const_cast<Module *>(this)->_getOrSetChild(name, NULL, true));
    }

    Module *Module::child(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::child()"); }
        Module *child = const_cast<Module *>(this)->_getOrSetChild(name);
        if(!child) { throw NotFoundException("Child not found in child()"); }
        return(child);
    }

    Module *Module::setChild(const QString &name, Module *mod) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Module::setChild()"); }
        if(!mod) { throw NullPointerException("NULL value passed to Module::setChild()"); }
        Module *child = _getOrSetChild(name, mod);
        if(!child) { throw NotFoundException("Child not found in Module::setChild()"); }
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
