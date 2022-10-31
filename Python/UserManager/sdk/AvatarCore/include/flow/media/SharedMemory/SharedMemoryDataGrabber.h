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

#ifndef __SHAREDMEMORYDataGRABBER__
#define __SHAREDMEMORYDataGRABBER__
#include <utils/SharedMemory.h>

#include "../AppSrc/IDataGrabber.h"
#include "../Resources/DataResourceHeader.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class ISharedMemoryDataGrabberListener {
public:
};

class AvaFlow_API SharedMemoryDataGrabber : public IDataGrabber {
private:
  DataResourceHeader *_header;
  DataFrame _frame;
  utils::ulong _lastFrame;
  utils::SharedMemory *_sharedMemroy;
  utils::uint _currentTop;

  utils::byte *_samplesPtr;

  std::string _Resource;

public:
  SharedMemoryDataGrabber();
  ~SharedMemoryDataGrabber();

  bool Open(const std::string &shmem);
  void Close();
  const std::string &GetName() const { return _Resource; }

  bool IsOpen() { return _header != 0; }

  virtual bool GrabFrame();
  virtual bool HasNewFrame() const;
  virtual utils::uint GetFrameLength() const;

  virtual const DataFrame *GetLastFrame() const;

  CoreResourceProvider GetResourceProvider();

  virtual void Lock();
  virtual void Unlock();
};

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYDataGRABBER__