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

#ifndef __ISingleton__
#define __ISingleton__

#include <typeinfo>

#include "avatarutils_config.h"
#include "base.h"

namespace utils {

template <typename T>
class ISingleton {
   protected:
    static T *m_instance;

   public:
    ISingleton() {
        FATAL_ERROR(m_instance != 0, (std::string("Duplicated Singleton: ") +
                                      std::string(typeid(T).name()))
                                         .c_str());
        m_instance = static_cast<T *>(this);
    }
    virtual ~ISingleton() { m_instance = 0; }
    static T &getInstance() {
        FATAL_ERROR(m_instance == 0,
                    (std::string("Instance object not created: ") +
                     std::string(typeid(T).name()))
                        .c_str());
        return *m_instance;
    }
    static bool isExist() { return m_instance != 0; }
    static T *getInstancePtr() {
        FATAL_ERROR(m_instance == 0,
                    (std::string("Instance object not created: ") +
                     std::string(typeid(T).name()))
                        .c_str())
        return m_instance;
    }

    static void DestroyInstance() {
        if (!m_instance) return;
        delete m_instance;
        m_instance = 0;
    }
};
template <typename T>
T *ISingleton<T>::m_instance = 0;

}  // namespace utils

#endif  //__ISingleton__