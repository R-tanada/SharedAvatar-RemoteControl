

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

#ifndef __DeviceMonitorManager__
#define __DeviceMonitorManager__

#include "flow/avatarflow_config.h"
#include "utils/IDelegate.h"
#include "utils/ISingleton.h"
#include "utils/ListenerContainer.h"

#include <list>

namespace avatarflow {
namespace media {

class IDeviceDetails;
class AudioSrcDeviceDetails;
class AudioSinkDeviceDetails;
class VideoSrcDeviceDetails;

class DeviceMonitorManagerImpl;

class IDeviceMonitorListener {
public:
  virtual ~IDeviceMonitorListener() {}
  virtual void OnDeviceAdded(IDeviceDetails *device) {}
  virtual void OnDeviceRemoved(IDeviceDetails *device) {}
};

class AvaFlow_API DeviceMonitorManager
    : public utils::ISingleton<DeviceMonitorManager>,
      public utils::ListenerContainer<IDeviceMonitorListener *> {
public:
protected:
  friend class DeviceMonitorManagerImpl;

  DeviceMonitorManagerImpl *_impl;

  DECLARE_FIRE_METHOD(OnDeviceAdded, (IDeviceDetails * device), (device))
  DECLARE_FIRE_METHOD(OnDeviceRemoved, (IDeviceDetails * device), (device))

public:
  DeviceMonitorManager(/* args */);
  virtual ~DeviceMonitorManager();

  virtual bool Start();
  virtual bool Stop();
  virtual void Refresh();

  const std::list<AudioSrcDeviceDetails *> &GetAudioSourceList();
  const std::list<AudioSinkDeviceDetails *> &GetAudioSinkList();
  const std::list<VideoSrcDeviceDetails *> &GetVideoSourceList();

  VideoSrcDeviceDetails *GetVideoSource(const std::string &id);
  AudioSrcDeviceDetails *GetAudioSource(const std::string &id);
  AudioSinkDeviceDetails *GetAudioSink(const std::string &id);
};

} // namespace media
} // namespace avatarflow

#endif //__DeviceMonitorManager__Width