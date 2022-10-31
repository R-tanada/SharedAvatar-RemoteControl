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

#ifndef __SharedMemoryAudioWriter__
#define __SharedMemoryAudioWriter__

#include <utils/GUID.h>

#include "../Resources/AudioResourceHeader.h"
#include "ISharedMemoryResourceWriter.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API SharedMemoryAudioWriter : public ISharedMemoryResourceWriter {
public:
private:
  AudioResourceHeader *_header;
  // AudioFrame _frame;
  utils::ulong _lastFrame;

  utils::byte *_samplesPtr;
  utils::ulong _memorySize;
  std::string _Resource;
  utils::SharedMemory *_sharedMemroy;

public:
  SharedMemoryAudioWriter();
  virtual ~SharedMemoryAudioWriter();

  virtual IResourceHeader *GetBaseHeader() { return GetHeader(); }
  const std::string &GetName() { return _Resource; }

  bool
  Open(const std::string &shmem, utils::uint size, bool closeIfExists = true,
       CoreResourceProvider provider = CoreResourceProvider::Provider_Local);
  void Close();

  utils::uint GetDataSize() {
    if (!_sharedMemroy)
      return 0;
    return _sharedMemroy->GetSize() - sizeof(AudioResourceHeader);
  }
  AudioFrame *GetFrame() { return &_header->frame; }

  AudioResourceHeader *GetHeader() { return _header; }

  void UpdateSharedMemory(bool lock = true);
  void Lock();
  void Unlock();

  int WriteData(const utils::byte *data, utils::uint len, bool lock);

  utils::shmem_mutex_type *Mutex();
};

} // namespace media
} // namespace avatarflow

#endif //__SharedMemoryAudioWriter__