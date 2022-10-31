
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

#ifndef __DELEGATEMANAGER__
#define __DELEGATEMANAGER__

#include "IDelegateContainer.h"
#include "ISingleton.h"

namespace utils {

typedef std::map<std::string, GCPtr<ObjectDelegate>> DelegateMap;

class AvaUtils_API DelegateManager : public ISingleton<DelegateManager> {
   private:
   protected:
    DelegateMap m_delegates;

   public:
    DelegateManager();
    virtual ~DelegateManager();

    void registerDelegate(const GCPtr<ObjectDelegate> &d);
    GCPtr<ObjectDelegate> getDelegate(const std::string &name);
    void removeDelegate(const std::string &name);

    void clear();
};

}  // namespace utils

#endif
