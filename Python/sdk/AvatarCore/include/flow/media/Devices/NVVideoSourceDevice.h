

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

#ifndef __NVVIDEOSOURCEDEVICE__
#define __NVVIDEOSOURCEDEVICE__

#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class NVVideoSourceDeviceImpl;

class NVVideoSourceDevice : public IVideoSourceDevice {
  DECLARE_RTTI;

  static const utils::GUID PATH_IMUData;

private:
  NVVideoSourceDeviceImpl *_impl;

  DECLARE_PROPERTY_TYPE(CameraIndex, int, ); // Capture width
  DECLARE_PROPERTY_TYPE(Width, int, );       // Capture width
  DECLARE_PROPERTY_TYPE(Height, int, );      // Capture height
  DECLARE_PROPERTY_TYPE(Framerate, int, );   // Capture height
  DECLARE_PROPERTY_TYPE(FlipMethod, int, );
  DECLARE_PROPERTY_TYPE(CropWidth, int, );
  DECLARE_PROPERTY_TYPE(CropHeight, int, );
  DECLARE_PROPERTY_TYPE(CropTop, int, );
  DECLARE_PROPERTY_TYPE(CropRight, int, );
  DECLARE_PROPERTY_TYPE(MinExposure, utils::uint, );
  DECLARE_PROPERTY_TYPE(MaxExposure, utils::uint, );
  DECLARE_PROPERTY_TYPE(MinGain, utils::uint, );
  DECLARE_PROPERTY_TYPE(MaxGain, utils::uint, );
  DECLARE_PROPERTY_TYPE(MinISPGain, utils::uint, );
  DECLARE_PROPERTY_TYPE(MaxISPGain, utils::uint, );
  DECLARE_PROPERTY_TYPE(EnableEdgeEnhancement, bool, );
  DECLARE_PROPERTY_TYPE(EdgeEnhancement, float, );
  DECLARE_PROPERTY_TYPE(EnableNoiseEnhancement, bool, );
  DECLARE_PROPERTY_TYPE(NoiseEnhancement, float, );
  DECLARE_PROPERTY_TYPE(AWBLock, bool, );
  DECLARE_PROPERTY_TYPE(AELock, bool, );
  DECLARE_PROPERTY_TYPE(ExposureCompensation, float, );
  DECLARE_PROPERTY_TYPE(Saturation, float, );
  DECLARE_PROPERTY_TYPE(WBMode, std::string, );
  DECLARE_PROPERTY_TYPE(AntiBanding, int, );

  DECLARE_SETGET_PROP(CameraIndex, int);
  DECLARE_SETGET_PROP(Width, int);
  DECLARE_SETGET_PROP(Height, int);
  DECLARE_SETGET_PROP(Framerate, int);
  DECLARE_SETGET_PROP(FlipMethod, int);
  DECLARE_SETGET_PROP(CropWidth, int);
  DECLARE_SETGET_PROP(CropHeight, int);
  DECLARE_SETGET_PROP(CropTop, int);
  DECLARE_SETGET_PROP(CropRight, int);
  DECLARE_SETGET_PROP(MinExposure, utils::uint);
  DECLARE_SETGET_PROP(MaxExposure, utils::uint);
  DECLARE_SETGET_PROP(MinGain, utils::uint);
  DECLARE_SETGET_PROP(MaxGain, utils::uint);
  DECLARE_SETGET_PROP(MinISPGain, utils::uint);
  DECLARE_SETGET_PROP(MaxISPGain, utils::uint);
  DECLARE_SETGET_PROP(EnableEdgeEnhancement, bool);
  DECLARE_SETGET_PROP(EdgeEnhancement, float);
  DECLARE_SETGET_PROP(EnableNoiseEnhancement, bool);
  DECLARE_SETGET_PROP(NoiseEnhancement, float);
  DECLARE_SETGET_PROP(AWBLock, bool);
  DECLARE_SETGET_PROP(AELock, bool);
  DECLARE_SETGET_PROP(ExposureCompensation, float);
  DECLARE_SETGET_PROP(Saturation, float);
  DECLARE_SETGET_PROP(WBMode, std::string);
  DECLARE_SETGET_PROP(AntiBanding, int);

  void _validateValues();

public:
  NVVideoSourceDevice(/* args */);
  virtual ~NVVideoSourceDevice();

  virtual bool Open();
  virtual void Close();
  virtual bool IsReady();
  virtual std::string BuildPipeline(const std::string &name);
  virtual bool UpdateSettings();
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(NVVideoSourceDevice, "nvargus")

} // namespace media
} // namespace avatarflow

#endif //__NVVIDEOSOURCEDEVICE__