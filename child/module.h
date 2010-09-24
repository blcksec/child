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

    class TaggedModule {
    public:
        TaggedModule() { throw RuntimeException("Cannot construct a NULL TaggedModule"); }
        TaggedModule(const QString &t, Module *const m);
        QString tag;
        Module *module;
    };

    inline bool operator==(const TaggedModule &a, const TaggedModule &b) {
        return(a.module == b.module && a.tag == b.tag);
    }

    inline uint qHash(const TaggedModule &key) {
        return qHash(key.tag) ^ ::qHash(key.module);
    }

    typedef QList<Module *> ModuleList;
    typedef QHash<QString, Module *> ModuleHash;
    typedef QHashIterator<QString, Module *> ModuleHashIterator;
    typedef QMultiHash<QString, Module *> ModuleMultiHash;
    typedef QSet<Module *> ModuleSet;
    typedef QQueue<Module *> ModuleQueue;
    typedef QList<TaggedModule> TaggedModuleList;
    typedef QSet<TaggedModule> TaggedModuleSet;
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
        void forgetAllForks();

        ModuleList extensions() const { return(_extensions ? ModuleList(*_extensions) : ModuleList()); }
        ModuleList extendedModules() const { return(_extendedModules ? ModuleList(*_extendedModules) : ModuleList()); }
        bool hasExtension(Module *mod) const;
        void addExtension(Module *mod);
        void prependExtension(Module *mod);
        void removeExtension(Module *mod);
        void removeAllExtensions();
        void forgetAllExtendedModules();

        const ModuleMultiHash parents() const;
        const ModuleHash children() const { return(_children ? ModuleHash(*_children) : ModuleHash()); }
        bool hasDirectParent(Module *mod) const;
        void addParent(const QString &tag, Module *mod);
        void removeParent(const QString &tag, Module *mod);
        void removeAllParents();
        bool hasDirectChild(const QString &tag) const;
        Module *directChild(const QString &tag) const;
        Module *addDirectChild(const QString &tag, Module *mod);
        Module *setDirectChild(const QString &tag, Module *mod);
        void removeDirectChild(const QString &tag);
        void forgetAllDirectChildren();
        Module *_getOrSetChild(const QString &tag, Module *setValue = NULL, bool returnThisIfFound = false);
        bool hasChild(const QString &tag) const;
        Module *child(const QString &tag) const;
        Module *setChild(const QString &tag, Module *mod);
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
        TaggedModuleSet *_parents;
        ModuleHash *_children; // backlink cache
        bool _isVirtual : 1;
    };
}

#endif // MODULE_H
