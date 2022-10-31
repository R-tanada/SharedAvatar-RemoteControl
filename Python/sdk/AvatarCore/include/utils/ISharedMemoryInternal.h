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

#ifndef __ISHAREDMEMORYINTERNAL__
#define __ISHAREDMEMORYINTERNAL__

#include "ISingleton.h"

namespace utils {

class AvaUtils_API ISharedMemoryInternal : public utils::ISingleton<ISharedMemoryInternal> {
   public:
    static void CreateInstance();

    virtual ~ISharedMemoryInternal();
    virtual void *CreateSharedMemory(const std::string &name, int size) = 0;

    virtual void *OpenSharedMemory(const std::string &name) = 0;
    virtual void *GetSharedMemoryReadyonly(const std::string &name) = 0;

    virtual bool DestroySharedMemory(const std::string &name) = 0;
    virtual bool IsSharedMemoryCreated(const std::string &name) = 0;
};
}  // namespace utils

#endif  //__ISHAREDMEMORYINTERNAL__