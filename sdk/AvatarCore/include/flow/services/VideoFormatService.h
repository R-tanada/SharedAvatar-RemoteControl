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

#ifndef __VIDEOFORMATSERVICE__
#define __VIDEOFORMATSERVICE__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class VideoFrame;
} // namespace media
namespace services {

class VideoFormatServiceImpl;
class AvaFlow_API VideoFormatService : public IService {
public:
  DECLARE_RTTI
protected:
  VideoFormatServiceImpl *_impl;

public:
  VideoFormatService(/* args */);
  virtual ~VideoFormatService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  virtual void LoadSettings(const JSONObject &settings);

  virtual void OnSettingsUpdated(const JSONObject &settings) {}
};
MAKE_SERVICE_FACTORY(VideoFormatService, "VideoFormat");

} // namespace services
} // namespace avatarflow

#endif //__VIDEOFORMATSERVICE__