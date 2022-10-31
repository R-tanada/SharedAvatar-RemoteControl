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

#ifndef __AppAudioSOURCEDEVICE__
#define __AppAudioSOURCEDEVICE__

#include "../AppSrc/AudioAppSrcHandler.h"
#include "../AppSrc/IAudioGrabber.h"
#include "IAudioSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API AppAudioSourceDevice : public IAudioSourceDevice,
                                         public IAudioGrabber {
  DECLARE_RTTI;

private:
  std::string _name;

  AudioAppSrcHandler *_appsrcHandler;
  IAudioGrabber *_grabber;

public:
  AppAudioSourceDevice(/* args */);
  ~AppAudioSourceDevice();
  virtual void AttachDataGrabber(IAudioGrabber *g) { _grabber = g; }

  virtual AudioFormat GetAudioFormat();
  virtual bool IsAudioBigEndian();
  virtual utils::uint GetSamplingRate();
  virtual utils::uint GetSamplesLength();
  virtual utils::uint GetChannelsCount();

  virtual bool GrabNoCopy(AudioFrame *f);
  virtual bool GrabFrame();
  virtual bool HasNewFrame();
  virtual const AudioFrame *GetLastFrame() const;
  virtual utils::ulong GetBufferID();

  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_AUDIOSOURCEDEVICE_FACTORY(AppAudioSourceDevice, "app")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYDataSOURCEDEVICE__