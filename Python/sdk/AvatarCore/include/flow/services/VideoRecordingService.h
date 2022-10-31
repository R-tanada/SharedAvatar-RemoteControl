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

#ifndef __VIDEORECORDINGSERVICE__
#define __VIDEORECORDINGSERVICE__

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

class VideoRecordingServiceImpl;
class AvaFlow_API VideoRecordingService : public IService {
public:
  DECLARE_RTTI
  DECLARE_PROPERTY_TYPE(Path, std::string, );
  DECLARE_SETGET_PROP(Path, std::string);

protected:
  VideoRecordingServiceImpl *_impl;

public:
  VideoRecordingService(/* args */);
  virtual ~VideoRecordingService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  media::GstPipeline *GetGstPipeline();

  utils::uint GetFPS();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(VideoRecordingService, "VideoRecording")

} // namespace services
} // namespace avatarflow

#endif //__VIDEORECORDINGSERVICE__