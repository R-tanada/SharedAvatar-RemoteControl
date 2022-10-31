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

#ifndef __AppVideoSourceDevice__
#define __AppVideoSourceDevice__

#include <utils/TypedProperty.h>
#include <utils/point2D.h>

#include "../AppSrc/VideoAppSrcHandler.h"
#include "../helpers/PixelFormat.h"
#include "IVideoSourceDevice.h"

namespace avatarflow {
namespace media {

class IVideoGrabber;

class AvaFlow_API AppVideoSourceDevice : public IVideoSourceDevice,
                                         public IAppSrcHandlerListener {
  DECLARE_RTTI;

private:
  IVideoGrabber *_videoGrabber;
  void *_capsElem;

  utils::vector2di _currSize;
  PixelFormat _currFormat;

  bool _open;
  bool _firstSample;

  VideoAppSrcHandler *_appsrcHandler;

  void _UpdateCaps();

public:
  AppVideoSourceDevice(/* args */);
  virtual ~AppVideoSourceDevice();

  // set your custom video grabber implementation object
  void SetVideoGrabber(IVideoGrabber *g) { _videoGrabber = g; }

  virtual bool Open();
  virtual void Close();
  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  virtual void OnNewSampleRequested(IAppSrcHandler *);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(AppVideoSourceDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__AppVideoSourceDevice__