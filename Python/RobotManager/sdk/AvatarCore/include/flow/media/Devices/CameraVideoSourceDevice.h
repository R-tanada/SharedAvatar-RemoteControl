

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

#ifndef __CAMERAVIDEOSOURCEDEVICE__
#define __CAMERAVIDEOSOURCEDEVICE__

#include "DeviceCaps.h"
#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {

enum EVideoFlipMethod {
  VideoFlip_None = 0,
  VideoFlip_CW = 1,
  VideoFlip_Rotate180 = 2,
  VideoFlip_CCW = 3,
  VideoFlip_HorizontalFlip = 4,
  VideoFlip_VerticalFlip = 5,
  VideoFlip_UpperLeftDiag = 6,
  VideoFlip_UpperRightDiag = 7,
  VideoFlip_Automatic = 8,
};

class CameraVideoSourceDeviceImpl;
class AvaFlow_API CameraVideoSourceDevice : public IVideoSourceDevice {
  DECLARE_RTTI;

private:
  CameraVideoSourceDeviceImpl *_impl;

  DECLARE_PROPERTY_TYPE(CameraIndex, std::string, ); // Capture width
  DECLARE_PROPERTY_TYPE(Width, int, );               // Capture width
  DECLARE_PROPERTY_TYPE(Height, int, );              // Capture height
  DECLARE_PROPERTY_TYPE(Framerate, int, );           // Capture height
  DECLARE_PROPERTY_TYPE(CaptureType, std::string, ); // Raw/JPEG
  DECLARE_PROPERTY_TYPE(FlipMethod, int, );
  DECLARE_PROPERTY_TYPE(CropTop, int, );
  DECLARE_PROPERTY_TYPE(CropRight, int, );
  DECLARE_PROPERTY_TYPE(CropWidth, int, );
  DECLARE_PROPERTY_TYPE(CropHeight, int, );

  DECLARE_SETGET_PROP(CameraIndex, std::string);
  DECLARE_SETGET_PROP(Width, int);
  DECLARE_SETGET_PROP(Height, int);
  DECLARE_SETGET_PROP(Framerate, int);
  DECLARE_SETGET_PROP(CaptureType, std::string);
  DECLARE_SETGET_PROP(FlipMethod, int);
  DECLARE_SETGET_PROP(CropTop, int);
  DECLARE_SETGET_PROP(CropRight, int);
  DECLARE_SETGET_PROP(CropWidth, int);
  DECLARE_SETGET_PROP(CropHeight, int);

public:
  CameraVideoSourceDevice(/* args */);
  ~CameraVideoSourceDevice();

  virtual void Close();
  virtual bool IsReady();
  virtual std::string BuildPipeline(const std::string &name);
  virtual bool UpdateSettings();
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(CameraVideoSourceDevice, "camera")

} // namespace media
} // namespace avatarflow

#endif //__CAMERAVIDEOSOURCEDEVICE__