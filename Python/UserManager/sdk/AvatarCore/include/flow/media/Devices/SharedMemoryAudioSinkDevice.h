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

#ifndef __SHAREDMEMORYAudioSINKDEVICE__
#define __SHAREDMEMORYAudioSINKDEVICE__

#include <utils/TypedProperty.h>

#include "../AppSink/ShmemAudioAppSinkHandler.h"
#include "../Resources/AudioFrame.h"
#include "../SharedMemory/SharedMemoryAudioWriter.h"
#include "IAudioSinkDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryAudioGrabber;

class AvaFlow_API SharedMemoryAudioSinkDevice : public IAudioSinkDevice {
  DECLARE_RTTI;

private:
  media::ShmemAudioAppSinkHandler *_handler;
  std::string _Resource;
  std::string _name;
  int _size;
  CoreResourceProvider _provider;

  DECLARE_PROPERTY_TYPE(Resource, std::string, );
  DECLARE_PROPERTY_TYPE(Size, int, );

  DECLARE_SETGET_PROP(Resource, std::string);
  DECLARE_SETGET_PROP(Size, int);

public:
  SharedMemoryAudioSinkDevice(/* args */);
  ~SharedMemoryAudioSinkDevice();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  void SetProvider(CoreResourceProvider provider);
};

MAKE_AUDIOSINKDEVICE_FACTORY(SharedMemoryAudioSinkDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYAudioSINKDEVICE__