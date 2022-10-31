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

#ifndef __SharedMemoryVideoWriter__
#define __SharedMemoryVideoWriter__

#include <utils/GUID.h>

#include "../Resources/VideoResourceHeader.h"
#include "ISharedMemoryResourceWriter.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API SharedMemoryVideoWriter : public ISharedMemoryResourceWriter {
public:
private:
  VideoResourceHeader *_header;
  // VideoFrame _frame;
  utils::ulong _lastFrame;

  utils::byte *_pixelsPtr;
  std::string _Resource;
  utils::SharedMemory *_sharedMemroy;

  bool _created;

  void _Clean();

public:
  SharedMemoryVideoWriter();
  virtual ~SharedMemoryVideoWriter();

  virtual IResourceHeader *GetBaseHeader() { return GetHeader(); }
  const std::string &GetName() { return _Resource; }

  bool
  Open(const std::string &shmem, utils::uint size, bool closeIfExists = true,
       CoreResourceProvider provider = CoreResourceProvider::Provider_Local);
  void Close();
  const std::string &GetName() const { return _Resource; }

  utils::uint GetDataSize() {
    if (!_sharedMemroy)
      return 0;
    return _sharedMemroy->GetSize();
  }
  VideoFrame *GetFrame() { return &_header->frame; }
  VideoResourceHeader *GetHeader() { return _header; }

  void UpdateSharedMemory(bool lock = true);
  void Lock();
  void Unlock();

  utils::shmem_mutex_type *Mutex();
  utils::SharedMemory *GetMemory() { return _sharedMemroy; }
};

} // namespace media
} // namespace avatarflow

#endif //__SharedMemoryVideoWriter__