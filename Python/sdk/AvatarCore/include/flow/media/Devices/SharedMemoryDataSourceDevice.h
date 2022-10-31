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

#ifndef __SHAREDMEMORYDataSOURCEDEVICE__
#define __SHAREDMEMORYDataSOURCEDEVICE__

#include <utils/TypedProperty.h>

#include "../AppSrc/DataAppSrcHandler.h"
#include "IDataSourceDevice.h"

namespace avatarflow {
namespace media {
class SharedMemoryDataGrabber;

class AvaFlow_API SharedMemoryDataSourceDevice : public IDataSourceDevice {
  DECLARE_RTTI;

private:
  std::string _Resource;

  SharedMemoryDataGrabber *_shmem;

  DataAppSrcHandler *_appsrcHandler;

  DECLARE_PROPERTY_TYPE(Resource, std::string, ); // Raw/JPEG

  DECLARE_SETGET_PROP(Resource, std::string);

public:
  SharedMemoryDataSourceDevice(/* args */);
  ~SharedMemoryDataSourceDevice();

  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_DATASOURCEDEVICE_FACTORY(SharedMemoryDataSourceDevice, "shmem")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYDataSOURCEDEVICE__