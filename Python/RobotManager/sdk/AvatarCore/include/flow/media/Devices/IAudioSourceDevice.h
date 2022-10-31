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

#ifndef __IAudioSOURCEDEVICE__
#define __IAudioSOURCEDEVICE__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>

#include <string>

#include "../GStreamer/GstPipeline.h"

namespace avatarflow {
namespace media {
class GstPipeline;

class IAudioSourceDevice : public utils::PropertySet {
  DECLARE_RTTI;

protected:
  std::string _srcName;
  GstPipeline *_pipeline;
  void *_appsrc;

private:
public:
  IAudioSourceDevice() {
    _appsrc = 0;
    _pipeline = 0;
  }
  virtual ~IAudioSourceDevice() {}
  virtual bool Open() { return true; }
  virtual void Close() {
    _pipeline = 0;
    _appsrc = 0;
  }

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
  /**
   * @brief Updates the internal settings (after LoadSettings for
   * example)
   *
   * @return true: Success
   * @return false: Failure
   */
  virtual bool UpdateSettings() { return true; }
  virtual void LinkWithPipeline(GstPipeline *pipeline) {
    _appsrc = pipeline->GetElementByName(_srcName);
    _pipeline = pipeline;
  }
  /**
   * @brief Does this device requires the service to stop on EOS
   *
   * @return true
   * @return false
   */
  virtual bool StopOnEOS() { return false; }

  /**
   * @brief Get the Remaining Buffer Length in bytes
   *
   * @return int
   */
  virtual int GetRemainingBufferLength() { return 0; }

  /**
   * @brief Get the Remaining Buffer duration length in milliseconds
   *
   * @return int
   */
  virtual int GetRemainingBufferDuration() { return 0; }
};

class IAudioSourceDeviceFactory {
public:
  virtual ~IAudioSourceDeviceFactory() {}
  virtual std::string GetType() = 0;
  virtual IAudioSourceDevice *Create() = 0;
};

#define MAKE_AUDIOSOURCEDEVICE_FACTORY(type, name)                             \
  class type##Factory : public IAudioSourceDeviceFactory {                     \
  public:                                                                      \
    virtual std::string GetType() { return name; }                             \
    virtual IAudioSourceDevice *Create() { return new type(); }                \
  };

} // namespace media
} // namespace avatarflow

#endif //__IAudioSOURCEDEVICE__