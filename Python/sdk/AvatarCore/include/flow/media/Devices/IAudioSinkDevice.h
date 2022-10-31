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

#ifndef __IAudioSinkDevice__
#define __IAudioSinkDevice__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>

#include <string>

#include "../GStreamer/GstPipeline.h"

namespace avatarflow {
namespace media {
class GstPipeline;

class IAudioSinkDevice : public utils::PropertySet {
  DECLARE_RTTI;

protected:
  std::string _sinkName;
  GstPipeline *_pipeline;
  void *_appsink;

private:
public:
  IAudioSinkDevice() {
    _appsink = 0;
    _pipeline = 0;
  }
  virtual ~IAudioSinkDevice() {}
  virtual bool Open() { return true; }
  virtual void Close() {}

  virtual std::string BuildPipeline(const std::string &name,
                                    bool sync = false) {
    _sinkName = name;
    return "";
  }
  /**
   * @brief Updates the internal settings (after LoadSettings for
   * example)
   *
   * @return true: Success
   * @return false: Failure
   */
  virtual bool UpdateSettings() { return true; }
  virtual void LinkWithPipeline(GstPipeline *pipeline) {
    _appsink = pipeline->GetElementByName(_sinkName);
    _pipeline = pipeline;
  }
};

class IAudioSinkDeviceFactory {
public:
  virtual ~IAudioSinkDeviceFactory() {}
  virtual std::string GetType() = 0;
  virtual IAudioSinkDevice *Create() = 0;
};

#define MAKE_AUDIOSINKDEVICE_FACTORY(type, name)                               \
  class type##Factory : public IAudioSinkDeviceFactory {                       \
  public:                                                                      \
    virtual std::string GetType() { return name; }                             \
    virtual IAudioSinkDevice *Create() { return new type(); }                  \
  };

} // namespace media
} // namespace avatarflow

#endif //__IAudioSinkDevice__