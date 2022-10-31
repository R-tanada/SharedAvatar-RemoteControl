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

#ifndef __APPDATASOURCEDEVICE__
#define __APPDATASOURCEDEVICE__

#include "../AppSrc/DataAppSrcHandler.h"
#include "../AppSrc/IDataGrabber.h"
#include "IDataSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API AppDataSourceDevice : public IDataSourceDevice,
                                        public IDataGrabber {
  DECLARE_RTTI;

private:
  std::string _Resource;

  DataAppSrcHandler *_appsrcHandler;
  IDataGrabber *_grabber;

public:
  AppDataSourceDevice(/* args */);
  ~AppDataSourceDevice();
  virtual void AttachDataGrabber(IDataGrabber *g) { _grabber = g; }

  virtual bool GrabFrame();
  virtual bool HasNewFrame() const;
  virtual utils::uint GetFrameLength() const;
  virtual const DataFrame *GetLastFrame() const;

  virtual std::string BuildPipeline(const std::string &name);
  virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_DATASOURCEDEVICE_FACTORY(AppDataSourceDevice, "app")

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYDataSOURCEDEVICE__