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

#ifndef __AppAudioSinkDevice__
#define __AppAudioSinkDevice__

#include <utils/TypedProperty.h>

#include "../AppSink/ShmemAudioAppSinkHandler.h"
#include "../Resources/AudioFrame.h"
#include "../SharedMemory/SharedMemoryAudioWriter.h"
#include "IAudioSinkDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryAudioGrabber;

class AvaFlow_API AppAudioSinkDevice : public IAudioSinkDevice {
  DECLARE_RTTI;

private:
  media::ShmemAudioAppSinkHandler *_handler;
  std::string _Resource;
  std::string _name;
  utils::uint _size;

  DECLARE_PROPERTY_TYPE(Resource, std::string, );

  DECLARE_SETGET_PROP(Resource, std::string);

public:
  AppAudioSinkDevice(/* args */);
  ~AppAudioSinkDevice();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync);
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_AUDIOSINKDEVICE_FACTORY(AppAudioSinkDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__AppAudioSinkDevice__