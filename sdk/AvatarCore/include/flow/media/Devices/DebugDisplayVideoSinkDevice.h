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

#ifndef __DebugDisplayVIDEOSinkDEVICE__
#define __DebugDisplayVIDEOSinkDEVICE__

#include <utils/TypedProperty.h>
#include <utils/point2D.h>

#include "IVideoSinkDevice.h"

namespace avatarflow {
namespace media {

class DebugDisplayVideoGrabber;

class AvaFlow_API DebugDisplayVideoSinkDevice : public IVideoSinkDevice {
  DECLARE_RTTI;

private:
public:
public:
  DebugDisplayVideoSinkDevice(/* args */);
  virtual ~DebugDisplayVideoSinkDevice();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync);
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_VIDEOSINKDEVICE_FACTORY(DebugDisplayVideoSinkDevice, "debug")

} // namespace media
} // namespace avatarflow

#endif //__DebugDisplayVIDEOSinkDEVICE__