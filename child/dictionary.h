#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "child/nativemethod.h"
#include "child/object.h"

namespace Child {
    class Dictionary : public Object {
    public:
        static Dictionary *root();

        virtual Dictionary *fork() const { return(_fork(this)); }
    private:
        static Dictionary *_root;
//        ModuleHash _hash;
        ModuleList _keys;
    };
}

#endif // DICTIONARY_H
