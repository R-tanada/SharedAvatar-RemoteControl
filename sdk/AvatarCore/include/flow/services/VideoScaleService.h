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

#ifndef __VIDEOSCALESERVICE__
#define __VIDEOSCALESERVICE__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class IVideoGrabber;
class VideoFrame;
class VideoSinkImageGrabber;
} // namespace media
namespace services {

class VideoScaleServiceImpl;
class AvaFlow_API VideoScaleService : public IService {
public:
  DECLARE_RTTI

  DECLARE_PROPERTY_SETGET(ScalerWidth, int);
  DECLARE_PROPERTY_SETGET(ScalerHeight, int);
  DECLARE_PROPERTY_SETGET(FrameRate, int);
  DECLARE_PROPERTY_SETGET(FlipMethod, int);

protected:
  VideoScaleServiceImpl *_impl;

public:
  VideoScaleService(/* args */);
  virtual ~VideoScaleService();

  bool Init(ServiceContext *context);

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  media::GstPipeline *GetGstPipeline();

  media::IVideoGrabber *GetImageGrabber();

  void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(VideoScaleService, "VideoScale")

} // namespace services
} // namespace avatarflow

#endif //__VIDEORECEIVERSERVICE__