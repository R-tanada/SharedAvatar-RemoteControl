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

#ifndef __SharedMemoryDataWriter__
#define __SharedMemoryDataWriter__

#include <utils/GUID.h>

#include "../Resources/DataResourceHeader.h"
#include "ISharedMemoryResourceWriter.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API SharedMemoryDataWriter : public ISharedMemoryResourceWriter {
public:
private:
  DataResourceHeader *_header;
  // DataFrame _frame;
  utils::ulong _lastFrame;

  utils::byte *_dataPtr;
  std::string _Resource;
  utils::SharedMemory *_sharedMemroy;

public:
  SharedMemoryDataWriter();
  ~SharedMemoryDataWriter();

  virtual IResourceHeader *GetBaseHeader() { return GetHeader(); }

  const std::string &GetName() { return _Resource; }

  bool
  Open(const std::string &shmem, utils::uint size, bool closeIfExists = true,
       CoreResourceProvider provider = CoreResourceProvider::Provider_Local);
  bool Open(const std::string &shmem);
  void Close();

  utils::uint GetDataSize() {
    if (!_sharedMemroy)
      return 0;
    return _sharedMemroy->GetSize() - sizeof(DataResourceHeader);
  }
  DataFrame *GetFrame() { return &_header->frame; }

  DataResourceHeader *GetHeader() { return _header; }

  int WriteData(const char *data, int len, bool lock);

  void UpdateSharedMemory(bool lock = true);
  void Lock();
  void Unlock();

  utils::shmem_mutex_type *Mutex();
};

} // namespace media
} // namespace avatarflow

#endif //__SharedMemoryDataWriter__