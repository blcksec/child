#ifndef MODULE_H
#define MODULE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "child/toolbox.h"
#include "child/exception.h"

namespace Child {
    class Module;

    class TaggedModule {
    public:
        TaggedModule(const QString &t, Module *const m);
        const QString tag;
        Module *const module;
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
    typedef QSet<TaggedModule> TaggedModuleSet;
    typedef Module *(Module::*ModuleMethodPtr)();

    class Module {
    public:
        Module() { _moduleCount++; }
        virtual ~Module();
        static const long long int moduleCount() { return(_moduleCount); }

        const ModuleList modules() const { return(ModuleList(_modules)); }
        const ModuleList clones() const { return(ModuleList(_clones)); }
        void addModule(Module *mod);
        void prependModule(Module *mod);
        void removeModule(Module *mod);
        virtual Module *clone() const;

        const ModuleMultiHash parents() const;
        const ModuleHash children() const { return(ModuleHash(_children)); }
        void addParent(const QString &tag, Module *mod);
        void removeParent(const QString &tag, Module *mod, bool callDeleteIfOrphan = true);
        Module *child(const QString &tag) const;
        bool hasChild(const QString &tag) const;
        void setChild(const QString &tag, Module *mod);
        void removeChild(const QString &tag);
        void deleteIfOrphan() { if(_parents.empty()) { delete this; } }
        bool findChild(const QString &tag, Module *&own, Module *&rcv) const;
        bool _findChildInSelfAndParents(const QString &tag, Module *&own, Module *&rcv, ModuleSet &modSeen) const;
        bool _findChildInSelfAndModules(const QString &tag, Module *&own, ModuleSet &modSeen) const;

        const long long int uniqueID() { return(reinterpret_cast<long long int>(this)); }
        const QString uniqueHexID() { return(QString("0x%1").arg(uniqueID(), 0, 16)); }
        const QString inspect();

//        Module *const send(const QString &name);
//        Module *const fatalSend(const QString &name);

//        virtual Module *toText();

//        Module *print();

    private:
        ModuleList _modules;
        mutable ModuleList _clones; // cache
        TaggedModuleSet _parents;
        mutable ModuleHash _children; // cache
        static long long int _moduleCount;
    };
}

#endif // MODULE_H
