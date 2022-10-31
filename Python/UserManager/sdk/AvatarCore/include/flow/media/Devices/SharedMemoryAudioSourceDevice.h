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

#ifndef __SHAREDMEMORYAudioSOURCEDEVICE__
#define __SHAREDMEMORYAudioSOURCEDEVICE__

#include <utils/TypedProperty.h>

#include "../AppSrc/AudioAppSrcHandler.h"
#include "../Resources/AudioFrame.h"
#include "IAudioSourceDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryAudioGrabber;

class AvaFlow_API SharedMemoryAudioSourceDevice
    : public IAudioSourceDevice,
      public IAppSrcHandlerListener {
  DECLARE_RTTI;

private:
  std::string _Resource;

  SharedMemoryAudioGrabber *_shmem;
  void *_capsElem;

  AudioFormat _currFormat;
  int _currChannels;
  int _currRate;

  bool _open;
  bool _firstSample;

  AudioAppSrcHandler *_appsrcHandler;

  DECLARE_PROPERTY_TYPE(Resource, std::string, ); // Raw/JPEG

  DECLARE_SETGET_PROP(Resource, std::string);

  void _UpdateCaps();

public:
  SharedMemoryAudioSourceDevice(/* args */);
  ~SharedMemoryAudioSourceDevice();
  virtual bool Open();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  virtual int GetRemainingBufferLength();
  virtual int GetRemainingBufferDuration();

  virtual void OnNewSampleRequested(IAppSrcHandler *src);
};

MAKE_AUDIOSOURCEDEVICE_FACTORY(SharedMemoryAudioSourceDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYAudioSOURCEDEVICE__