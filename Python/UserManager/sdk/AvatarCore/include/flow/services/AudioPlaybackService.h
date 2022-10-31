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

#ifndef __AUDIOPLAYBACKSERVICE__
#define __AUDIOPLAYBACKSERVICE__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class IAudioGrabber;
class AudioFrame;
class AudioSinkFrameGrabber;
} // namespace media
namespace services {

class AudioPlaybackServiceImpl;
class AvaFlow_API AudioPlaybackService : public IService {
public:
  DECLARE_RTTI

  typedef utils::IDelegate1<void, media::AudioFrame *> *OnAudioFrameDelegate;
  OnAudioFrameDelegate OnAudioFrame;

  static const utils::GUID PATH_SetAutoAdjust;

protected:
  AudioPlaybackServiceImpl *_impl;

public:
  AudioPlaybackService(/* args */);
  virtual ~AudioPlaybackService();

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual void Update(float dt);

  media::GstPipeline *GetGstPipeline();
  virtual bool IsRunning();

  void SetVolume(float vol);
  float GetVolume();
  void Mute(bool m);
  bool IsMute();

  float GetRMS(int channel, bool normalized);
  float GetPeak(int channel);
  float GetDecay(int channel);

  int GetChannelsCount();

  virtual void LoadSettings(const JSONObject &settings);

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
  virtual void OnSettingsUpdated(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(AudioPlaybackService, "AudioPlayback")

} // namespace services
} // namespace avatarflow

#endif //__AUDIOPLAYBACKSERVICE__