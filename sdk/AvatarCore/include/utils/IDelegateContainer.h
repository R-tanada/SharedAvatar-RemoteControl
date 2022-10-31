/************************************************************************
 *                                                                      *
 * Copyright 2020 AvatarIn Inc. Tokyo, Japan. All Rights Reserved       *
 *                                                                      *
 *       _____               __               .___                      *
 *      /  _  \___  ______ _/  |______ _______|   | ____                *
 *     /  /_\  \  \/ |__  \\   __\__  \\_  __ \   |/    \               *
 *    /    |    \   / / __ \|  |  / __ \|  | \/   |   |  \              *
 *    \____|__  /\_/ (____  /__| (____  /__|  |___|___|  /              *
 *            \/          \/          \/               \/               *
 *                                                                      *
 * Developed by: Yamen Saraiji                                          *
 ************************************************************************/

#ifndef __IDELEGATECONTAINER__
#define __IDELEGATECONTAINER__

#include "IDelegate.h"
#include "atypes.h"

namespace utils {
class IObject;
typedef IObject *IObjectPtr;
typedef IDelegate2<void, IObjectPtr, PVOID> ObjectDelegate;
#define DECLARE_OBJECT_DELEGATE(Name) void Name(IObjectPtr p1, mray::PVOID p2)
#define ADD_DELEGATE(T, Name) \
    AddDelegate(newClassDelegate2(mT(#Name), this, &T::Name))

class IDelegateContainer {
   protected:
    typedef std::map<std::string, ObjectDelegate *> DelegateMap;
    DelegateMap m_delegates;

   public:
    IDelegateContainer() {}
    virtual ~IDelegateContainer() {
        DelegateMap::iterator it = m_delegates.begin();
        for (; it != m_delegates.end(); ++it) {
            delete it->second;
        }
        m_delegates.clear();
    }
    void AddDelegate(ObjectDelegate *d) { m_delegates[d->getName()] = d; }

    void CallDelegate(const std::string &name, IObject *p1, PVOID p2) {
        DelegateMap::iterator it = m_delegates.find(name);
        if (it == m_delegates.end()) return;
        (*(*it).second)(p1, p2);
    }
};

}  // namespace utils

#endif