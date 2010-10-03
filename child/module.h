#ifndef MODULE_H
#define MODULE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QQueue>

#include "child/toolbox.h"
#include "child/exception.h"

namespace Child {
    class Module;

    class NamedModule {
    public:
        NamedModule() { throw RuntimeException("Cannot construct a NULL NamedModule"); }
        NamedModule(const QString &t, Module *const m);
        QString name;
        Module *module;
    };

    inline bool operator==(const NamedModule &a, const NamedModule &b) {
        return(a.module == b.module && a.name == b.name);
    }

    inline uint qHash(const NamedModule &key) {
        return qHash(key.name) ^ ::qHash(key.module);
    }

    typedef QList<Module *> ModuleList;
    typedef QHash<QString, Module *> ModuleHash;
    typedef QHashIterator<QString, Module *> ModuleHashIterator;
    typedef QMultiHash<QString, Module *> ModuleMultiHash;
    typedef QSet<Module *> ModuleSet;
    typedef QQueue<Module *> ModuleQueue;
    typedef QList<NamedModule> NamedModuleList;
    typedef QSet<NamedModule> NamedModuleSet;
    typedef Module *(Module::*ModuleMethodPtr)();

    class Module {
    public:
        Module() :
            _baseModule(NULL), _forks(NULL), _extensions(NULL), _extendedModules(NULL),
            _parents(NULL), _children(NULL), _isVirtual(false) {
            _moduleCount++;
        }
        virtual ~Module();
        static const long long int moduleCount() { return(_moduleCount); }

        static Module *root();

        bool isVirtual() const { return(_isVirtual); }
        Module *setIsVirtual(bool value) { _isVirtual = value; return(this); }

        Module *baseModule() const { return(_baseModule); }
        void setBaseModule(Module *mod);
        void unsetBaseModule() { if(_baseModule) { _baseModule->_forks->removeOne(this); _baseModule = NULL; } }
        ModuleList forks() const { return(_forks ? ModuleList(*_forks) : ModuleList()); }
        bool isBasedOn(Module *mod) const;
    protected:
        template<typename T> static T *_fork(T *module) {
            T *fork = new T;
            fork->setBaseModule(module);
            return(fork);
        }
    public:
        virtual Module *fork() { return(_fork(this)); }
        void removeAllForks();

        ModuleList extensions() const { return(_extensions ? ModuleList(*_extensions) : ModuleList()); }
        ModuleList extendedModules() const { return(_extendedModules ? ModuleList(*_extendedModules) : ModuleList()); }
        bool hasExtension(Module *mod) const;
        void addExtension(Module *mod);
        void prependExtension(Module *mod);
        void removeExtension(Module *mod);
        void removeAllExtensions();
        void removeAllExtendedModules();

        const ModuleMultiHash parents() const;
        const ModuleHash children() const { return(_children ? ModuleHash(*_children) : ModuleHash()); }
        bool hasDirectParent(Module *mod) const;
        void addParent(const QString &name, Module *mod);
        void removeParent(const QString &name, Module *mod);
        void removeAllParents();
        bool hasDirectChild(const QString &name) const;
        Module *directChild(const QString &name) const;
        Module *addDirectChild(const QString &name, Module *mod);
        Module *setDirectChild(const QString &name, Module *mod);
        Module *addOrSetDirectChild(const QString &name, Module *mod);
        void removeDirectChild(const QString &name);
        void removeAllDirectChildren();
        Module *_getOrSetChild(const QString &name, Module *setValue = NULL, bool returnThisIfFound = false);
        bool hasChild(const QString &name) const;
        Module *child(const QString &name) const;
        Module *setChild(const QString &name, Module *mod);
        void deleteIfOrphan() { if(!_parents || _parents->empty()) { delete this; } }

        const long long int uniqueID() const { return(reinterpret_cast<long long int>(this)); }
        const QString uniqueHexID() const { return(QString("0x%1").arg(uniqueID(), 0, 16)); }
        const QString inspect() const;

//        Module *const send(const QString &name);
//        Module *const fatalSend(const QString &name);

//        virtual Module *toText();

//        Module *print();

    private:
        static long long int _moduleCount;
        static Module *_root;
        Module *_baseModule;
        ModuleList *_forks; // backlink cache
        ModuleList *_extensions;
        ModuleList *_extendedModules; // backlink cache
        NamedModuleSet *_parents;
        ModuleHash *_children; // backlink cache
        bool _isVirtual : 1;
    };
}

#endif // MODULE_H
