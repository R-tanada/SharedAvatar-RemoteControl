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

#ifndef __SHAREDMEMORYDataSINKDEVICE__
#define __SHAREDMEMORYDataSINKDEVICE__

#include <utils/TypedProperty.h>

#include "../AppSink/ShmemDataAppSinkHandler.h"
#include "../Resources/DataFrame.h"
#include "../SharedMemory/SharedMemoryDataWriter.h"
#include "IDataSinkDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryDataGrabber;

class AvaFlow_API SharedMemoryDataSinkDevice : public IDataSinkDevice {
  DECLARE_RTTI;

private:
  media::SharedMemoryDataWriter _shmemWriter;
  media::ShmemDataAppSinkHandler *_handler;
  std::string _Resource;
  std::string _name;
  utils::uint _size;
  CoreResourceProvider _provider;

  DECLARE_PROPERTY_TYPE(Resource, std::string, );

  DECLARE_SETGET_PROP(Resource, std::string);

public:
  SharedMemoryDataSinkDevice(/* args */);
  ~SharedMemoryDataSinkDevice();
  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync);
  virtual void LinkWithPipeline(GstPipeline *pipeline);

  void SetProvider(CoreResourceProvider provider);
};

MAKE_DATASINKDEVICE_FACTORY(SharedMemoryDataSinkDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYDataSINKDEVICE__