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

#ifndef __SHAREDMEMORYVIDEOSinkDEVICE__
#define __SHAREDMEMORYVIDEOSinkDEVICE__

#include <utils/TypedProperty.h>
#include <utils/point2D.h>

#include "../AppSink/ShmemVideoAppSinkHandler.h"
#include "../Resources/VideoFrame.h"
#include "../SharedMemory/SharedMemoryVideoWriter.h"
#include "IVideoSinkDevice.h"

namespace avatarflow {
namespace media {

class SharedMemoryVideoGrabber;

class AvaFlow_API SharedMemoryVideoSinkDevice : public IVideoSinkDevice,
                                                public IAppSinkHandlerListener {
  DECLARE_RTTI;

private:
  media::SharedMemoryVideoWriter _shmemWriter;
  media::ShmemVideoAppSinkHandler *_handler;
  std::string _Resource;
  std::string _name;
  utils::uint _size;
  std::string _format;
  CoreResourceProvider _provider;

public:
  DECLARE_PROPERTY_TYPE(Resource, std::string, );
  DECLARE_PROPERTY_TYPE(Format, std::string, );

  DECLARE_SETGET_PROP(Format, std::string);
  DECLARE_SETGET_PROP(Resource, std::string);

public:
  SharedMemoryVideoSinkDevice(/* args */);
  virtual ~SharedMemoryVideoSinkDevice();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  void SetProvider(CoreResourceProvider provider);

  virtual void OnSampleChanged(IAppSinkHandler *);
};

MAKE_VIDEOSINKDEVICE_FACTORY(SharedMemoryVideoSinkDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYVIDEOSinkDEVICE__