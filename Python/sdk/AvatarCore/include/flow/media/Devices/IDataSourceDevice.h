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

#ifndef __IDataSOURCEDEVICE__
#define __IDataSOURCEDEVICE__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>

#include <string>

#include "../GStreamer/GstPipeline.h"

namespace avatarflow {
namespace media {

class IDataSourceDevice : public utils::PropertySet {
  DECLARE_RTTI
protected:
  std::string _srcName;
  void *_appsrc;

public:
  IDataSourceDevice() { _appsrc = 0; }
  virtual ~IDataSourceDevice() {}
  virtual bool Open() { return true; }
  virtual void Close() {}

  /**
   * @brief Indicates if this device is ready for use (can output images yet or
   * not). Some devices may requires some time to start ouputing samples.
   * Override to report that
   *
   * @return true
   * @return false
   */
  virtual bool IsReady() { return true; }

  virtual std::string BuildPipeline(const std::string &name) {
    _srcName = name;
    return "";
  }
  virtual void LinkWithPipeline(GstPipeline *pipeline) {
    _appsrc = pipeline->GetElementByName(_srcName);
  }
  /**
   * @brief Does this device requires the service to stop on EOS
   *
   * @return true
   * @return false
   */
  virtual bool StopOnEOS() { return false; }
};

class IDataSourceDeviceFactory {
public:
  virtual ~IDataSourceDeviceFactory() {}
  virtual std::string GetType() = 0;
  virtual IDataSourceDevice *Create() = 0;
};

#define MAKE_DATASOURCEDEVICE_FACTORY(type, name)                              \
  class type##Factory : public IDataSourceDeviceFactory {                      \
  public:                                                                      \
    virtual std::string GetType() { return name; }                             \
    virtual IDataSourceDevice *Create() { return new type(); }                 \
  };
} // namespace media
} // namespace avatarflow

#endif //__IDataSOURCEDEVICE__