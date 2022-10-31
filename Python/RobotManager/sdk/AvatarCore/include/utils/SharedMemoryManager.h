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

#ifndef __SHAREDMEMORYMANAGER__
#define __SHAREDMEMORYMANAGER__

#include "ISingleton.h"
#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {

class SharedMemoryManagerImpl;
class SharedMemory;

class AvaUtils_API SharedMemoryManager
    : public ISingleton<SharedMemoryManager> {
private:
  SharedMemoryManagerImpl *_impl;

public:
  SharedMemoryManager(/* args */);
  ~SharedMemoryManager();

  bool Initialize(bool master, int maxCount = 128);
  SharedMemory *CreateSharedMemory(const std::string &name, int size,
                                   bool force, bool &created);
  SharedMemory *GetSharedMemory(const std::string &name);

  /**
   * @brief Remove reference from a shared memory by name, if refCountOnly is
   * set to false, then the shared memory will be forcefully destroyed and
   * closed
   *
   * @param name Name of the memory to remove/unref
   * @param refCountOnly If true, then the ref count will be reduced by one,
   * memory will be destroyed if ref count reached zero, or the this flag was
   * disabled
   * @return true Successfully unreferenced the memory
   * @return false Memory was not found
   */
  bool RemoveSharedMemory(const std::string &name, bool refCountOnly = true);

  bool IsMemoryExist(const std::string &name);
  uint GetMemorySize(const std::string &name);
  uint GetMemoryRefCount(const std::string &name);

  static void SetGlobalPrefix(const std::string &prefix);
  static const std::string &GetGlobalPrefix();
};

} // namespace utils

#define gSharedMemoryManager utils::SharedMemoryManager::getInstance()

#endif //__SHAREDMEMORYMANAGER__