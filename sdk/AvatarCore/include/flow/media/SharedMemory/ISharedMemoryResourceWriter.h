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

#ifndef __ISHAREDMEMORYRESOURCEWRITER__
#define __ISHAREDMEMORYRESOURCEWRITER__

#include <utils/SharedMemory.h>

#include "flow/media/Resources/IResourceHeader.h"

namespace avatarflow {
namespace media {

class ISharedMemoryResourceWriter {
private:
  /* data */
public:
  ISharedMemoryResourceWriter(/* args */) {}
  virtual ~ISharedMemoryResourceWriter() {}

  CoreResourceType GetResourceType() { return GetBaseHeader()->GetType(); }

  virtual const std::string &GetName() = 0;
  virtual bool Open(
      const std::string &shmem, utils::uint size, bool closeIfExists = true,
      CoreResourceProvider provider = CoreResourceProvider::Provider_Local) = 0;
  virtual void Close() = 0;

  virtual utils::uint GetDataSize() = 0;

  virtual IResourceHeader *GetBaseHeader() = 0;

  virtual void UpdateSharedMemory(bool lock = true) = 0;
  virtual void Lock() = 0;
  virtual void Unlock() = 0;

  virtual utils::shmem_mutex_type *Mutex() = 0;
};

} // namespace media
} // namespace avatarflow

#endif //__ISHAREDMEMORYRESOURCEWRITER__