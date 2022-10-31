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

#ifndef __IDELEGATESET__
#define __IDELEGATESET__

#include <list>

#include "IDelegate.h"

namespace utils {

typedef std::list<IDelegateEvent *> DelegateList;
typedef core::IteratorPair<DelegateList> DelegateListIterator;

class IDelegateSet {
   private:
   protected:
   public:
    IDelegateSet() {}
    virtual ~IDelegateSet() {}

    virtual void addDelegateEvent(IDelegateEvent *e) = 0;
    virtual DelegateListIterator getDelegateEventList() = 0;
    virtual IDelegateEvent *getDelegateEvent(const std::string &name) = 0;
};

}  // namespace utils

#endif  //___IDelegateSet___
