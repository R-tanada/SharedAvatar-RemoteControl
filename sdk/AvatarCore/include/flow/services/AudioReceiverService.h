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

#ifndef __AudioRECEIVERSERVICE__
#define __AudioRECEIVERSERVICE__

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

class AudioReceiverServiceImpl;
class AvaFlow_API AudioReceiverService : public IService {
public:
  DECLARE_RTTI

  typedef utils::IDelegate1<void, media::AudioFrame *> *OnAudioFrameDelegate;
  OnAudioFrameDelegate OnAudioFrame;

protected:
  AudioReceiverServiceImpl *_impl;

public:
  AudioReceiverService(/* args */);
  virtual ~AudioReceiverService();

  virtual bool StartService();
  virtual bool StopService();

  media::GstPipeline *GetGstPipeline();
  virtual bool IsRunning();

  utils::uint GetFPS();
  media::IAudioGrabber *GetAudioGrabber();
  media::AudioSinkFrameGrabber *GetSinkGrabber();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(AudioReceiverService, "AudioReceiver")

} // namespace services
} // namespace avatarflow

#endif //__AudioRECEIVERSERVICE__