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
        Module() : _isVirtual(false) { _moduleCount++; }
        virtual ~Module();
        static const long long int moduleCount() { return(_moduleCount); }

        static Module *root();

        bool isVirtual() const { return(_isVirtual); }
        Module *setIsVirtual(bool value) { _isVirtual = value; return(this); }

        const ModuleList modules() const { return(ModuleList(_modules)); }
        const ModuleList clones() const { return(ModuleList(_clones)); }
        bool hasDirectModule(Module *mod) const;
        void addModule(Module *mod);
        void prependModule(Module *mod);
        void removeModule(Module *mod);
    protected:
        template<typename T> static T *_fork(const T *module) {
            T *fork = new T;
            fork->addModule(const_cast<T *>(module));
            return(fork);
        }
    public:
        virtual Module *fork() const { return(_fork(this)); }

        const ModuleMultiHash parents() const;
        const ModuleHash children() const { return(ModuleHash(_children)); }
        bool hasDirectParent(Module *mod) const;
        void addParent(const QString &tag, Module *mod);
        void removeParent(const QString &tag, Module *mod, bool callDeleteIfOrphan = true);
        bool hasDirectChild(const QString &tag) const;
        Module *directChild(const QString &tag) const;
        Module *addDirectChild(const QString &tag, Module *mod);
        Module *setDirectChild(const QString &tag, Module *mod);
        void removeDirectChild(const QString &tag);
        Module *_getOrSetChild(const QString &tag, Module *setValue = NULL, bool returnThisIfFound = false);
        bool hasChild(const QString &tag) const;
        Module *child(const QString &tag) const;
        Module *setChild(const QString &tag, Module *mod);
        void deleteIfOrphan() { if(_parents.empty()) { delete this; } }

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
        ModuleList _modules;
        mutable ModuleList _clones; // cache
        TaggedModuleSet _parents;
        mutable ModuleHash _children; // cache
        bool _isVirtual : 1;
    };
}

#endif // MODULE_H
