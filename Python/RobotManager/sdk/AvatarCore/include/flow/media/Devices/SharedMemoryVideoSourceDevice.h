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

#ifndef __SHAREDMEMORYVIDEOSOURCEDEVICE__
#define __SHAREDMEMORYVIDEOSOURCEDEVICE__

#include <utils/TypedProperty.h>
#include <utils/point2D.h>

#include "../AppSrc/VideoAppSrcHandler.h"
#include "../helpers/PixelFormat.h"
#include "IVideoSourceDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryVideoGrabber;

class IVideoGrabber;

class AvaFlow_API SharedMemoryVideoSourceDevice
    : public IVideoSourceDevice,
      public IAppSrcHandlerListener {
  DECLARE_RTTI;

private:
  std::string _Resource;
  SharedMemoryVideoGrabber *_shmem;
  IVideoGrabber *_videoGrabber;
  void *_capsElem;

  utils::vector2di _currSize;
  PixelFormat _currFormat;

  bool _open;

  bool _firstSample;

  VideoAppSrcHandler *_appsrcHandler;

  DECLARE_PROPERTY_TYPE(Resource, std::string, ); // Raw/JPEG

  DECLARE_SETGET_PROP(Resource, std::string);

  void _UpdateCaps();

public:
  SharedMemoryVideoSourceDevice(/* args */);
  ~SharedMemoryVideoSourceDevice();

  virtual bool Open();
  virtual void Close();
  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  virtual void OnNewSampleRequested(IAppSrcHandler *);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(SharedMemoryVideoSourceDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYVIDEOSOURCEDEVICE__