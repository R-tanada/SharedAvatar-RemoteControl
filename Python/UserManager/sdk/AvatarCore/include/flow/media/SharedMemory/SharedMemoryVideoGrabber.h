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

#ifndef __SHAREDMEMORYVIDEOGRABBER__
#define __SHAREDMEMORYVIDEOGRABBER__
#include <utils/SharedMemory.h>

#include "../AppSrc/IVideoGrabber.h"
#include "../Resources/VideoResourceHeader.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class ISharedMemoryVideoGrabberListener {
public:
};

class AvaFlow_API SharedMemoryVideoGrabber : public IVideoGrabber {
private:
  VideoResourceHeader *_header;
  VideoFrame _frame;
  utils::ulong _lastFrame;
  utils::SharedMemory *_sharedMemroy;
  std::string _Resource;

  utils::byte *_pixelsPtr;

public:
  SharedMemoryVideoGrabber();
  ~SharedMemoryVideoGrabber();

  bool Open(const std::string &shmem);
  void Close();
  const std::string &GetName() const { return _Resource; }

  bool IsOpen() { return _header != 0; }

  virtual const utils::vector2di &GetFrameSize() { return _frame.GetSize(); }
  virtual PixelFormat GetImageFormat() { return _frame.GetFormat(); }

  virtual int GetFramesCount() { return 1; }
  virtual bool GrabFrame();
  virtual bool HasNewFrame();
  virtual utils::ulong GetBufferID(); // incremented once per frame
  virtual float GetCaptureFrameRate();

  virtual const VideoFrame *GetLastFrame() const;

  utils::SharedMemory *GetMemory() { return _sharedMemroy; }

  virtual void Lock();
  virtual void Unlock();

  CoreResourceProvider GetResourceProvider();
};

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYVIDEOGRABBER__