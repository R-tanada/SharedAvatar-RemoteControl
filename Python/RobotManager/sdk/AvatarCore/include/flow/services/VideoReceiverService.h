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

#ifndef __VIDEORECEIVERSERVICE__
#define __VIDEORECEIVERSERVICE__

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

class VideoReceiverServiceImpl;
class AvaFlow_API VideoReceiverService : public IService {
public:
  DECLARE_RTTI

  typedef utils::IDelegate1<void, media::VideoFrame *> *OnVideoFrameDelegate;
  OnVideoFrameDelegate OnVideoFrame;

protected:
  VideoReceiverServiceImpl *_impl;

public:
  VideoReceiverService(/* args */);
  virtual ~VideoReceiverService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  media::GstPipeline *GetGstPipeline();

  utils::uint GetFPS();
  media::IVideoGrabber *GetImageGrabber();
  media::VideoSinkImageGrabber *GetSinkGrabber();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(VideoReceiverService, "VideoReceiver")

} // namespace services
} // namespace avatarflow

#endif //__VIDEORECEIVERSERVICE__