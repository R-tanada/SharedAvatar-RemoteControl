

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

#ifndef __VideoCaptureService__
#define __VideoCaptureService__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class VideoFrame;
} // namespace media
namespace services {

class VideoCaptureServiceImpl;
class AvaFlow_API VideoCaptureService : public IService {
public:
  DECLARE_RTTI;

  typedef utils::IDelegate1<void, media::VideoFrame *> *OnVideoFrameDelegate;
  OnVideoFrameDelegate OnVideoFrame;

  static utils::GUID PATH_VideoStatus;
  DECLARE_PROPERTY_TYPE(HotStartup, bool, );
  DECLARE_SETGET_PROP(HotStartup, bool);

protected:
  VideoCaptureServiceImpl *_impl;

public:
  VideoCaptureService(/* args */);
  virtual ~VideoCaptureService();

  virtual bool StartService();
  virtual bool StopService();
  virtual void Update(float dt);
  virtual bool IsRunning();

  media::GstPipeline *GetGstPipeline();
  utils::uint GetFPS();

  virtual bool Init(ServiceContext *context);
  virtual void LoadSettings(const JSONObject &settings);

  virtual void OnSettingsUpdated(const JSONObject &settings);

  static void PublishVideoSize(const utils::GUID &source,
                               const utils::GUID &targetVideo, int width,
                               int height, int fps);
};
MAKE_SERVICE_FACTORY(VideoCaptureService, "VideoCapture");

} // namespace services
} // namespace avatarflow

#endif //__VideoCaptureService__Width