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

#ifndef __IDataSinkDevice__
#define __IDataSinkDevice__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>

#include <string>

#include "../GStreamer/GstPipeline.h"

namespace avatarflow {
namespace media {

class IDataSinkDevice : public utils::PropertySet {
  DECLARE_RTTI
protected:
  std::string _sinkName;
  GstPipeline *_pipeline;
  void *_appsink;

public:
  IDataSinkDevice() {
    _appsink = 0;
    _pipeline = 0;
  }
  virtual ~IDataSinkDevice() {}
  virtual bool Open() { return true; }
  virtual void Close() {}

  virtual std::string BuildPipeline(const std::string &name,
                                    bool sync = false) {
    _sinkName = name;
    return "";
  }
  virtual void LinkWithPipeline(GstPipeline *pipeline) {
    _appsink = pipeline->GetElementByName(_sinkName);
    _pipeline = pipeline;
  }
};

class IDataSinkDeviceFactory {
public:
  virtual ~IDataSinkDeviceFactory() {}
  virtual std::string GetType() = 0;
  virtual IDataSinkDevice *Create() = 0;
};

#define MAKE_DATASINKDEVICE_FACTORY(type, name)                                \
  class type##Factory : public IDataSinkDeviceFactory {                        \
  public:                                                                      \
    virtual std::string GetType() { return name; }                             \
    virtual IDataSinkDevice *Create() { return new type(); }                   \
  };
} // namespace media
} // namespace avatarflow

#endif //__IDataSinkDevice__