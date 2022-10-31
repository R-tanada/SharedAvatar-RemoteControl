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

#ifndef __IDELEGATEEVENT__
#define __IDELEGATEEVENT__

#include <string>

namespace utils {

class IDelegateEvent {
   protected:
    std::string m_name;

   public:
    IDelegateEvent(const std::string &name) : m_name(name) {}

    void setName(const std::string &name) { m_name = name; }
    const std::string &getName() const { return m_name; }
};

}  // namespace utils

#endif  //___IDelegateEvent___
