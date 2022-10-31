

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

#ifndef __AudioCaptureService__
#define __AudioCaptureService__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class AudioFrame;
} // namespace media
namespace services {

class AudioCaptureServiceImpl;
class AvaFlow_API AudioCaptureService : public IService {
public:
  DECLARE_RTTI;

  static const utils::GUID PATH_SetAutoAdjust;

protected:
  AudioCaptureServiceImpl *_impl;

public:
  AudioCaptureService(/* args */);
  virtual ~AudioCaptureService();

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();
  virtual void Update(float dt);

  media::GstPipeline *GetGstPipeline();

  void SetVolume(float vol);
  float GetVolume();
  void Mute(bool m);
  bool IsMute();
  void AutoAdjust(bool a);
  bool IsAutoAdjust();

  virtual void LoadSettings(const JSONObject &settings);
  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
  virtual void OnSettingsUpdated(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(AudioCaptureService, "AudioCapture");

} // namespace services
} // namespace avatarflow

#endif //__AudioCaptureService__Width