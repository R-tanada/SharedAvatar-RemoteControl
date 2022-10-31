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

#ifndef __SPEAKERAUDIOSINKDEVICE__
#define __SPEAKERAUDIOSINKDEVICE__

#include "DeviceMonitorManager.h"
#include "IAudioSinkDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API SpeakerAudioSinkDevice : public IAudioSinkDevice,
                                           public IDeviceMonitorListener {
  DECLARE_RTTI;

private:
  struct Settings {
    Settings() : _device(""), _samplerate(0), _channels(0) {}
    std::string _device;
    utils::uint _samplerate;
    utils::uint _channels;
  };
  void *_capsfilter;
  Settings _oldSettings, _settings;
  IDeviceDetails *_selectedDevice;

  DECLARE_PROPERTY_TYPE(Device, std::string, ); // Capture device
  DECLARE_PROPERTY_TYPE(SampleRate, utils::uint, );
  DECLARE_PROPERTY_TYPE(Channels, utils::uint, );

  DECLARE_SETGET_PROP(Device, std::string); // Capture device
  DECLARE_SETGET_PROP(SampleRate, utils::uint);
  DECLARE_SETGET_PROP(Channels, utils::uint);

  bool _deviceChanged(IDeviceDetails *device);

public:
  SpeakerAudioSinkDevice(/* args */);
  ~SpeakerAudioSinkDevice();

  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name, bool sync = false);
  virtual bool UpdateSettings();
  virtual void LinkWithPipeline(GstPipeline *pipeline);
  virtual void OnDeviceAdded(IDeviceDetails *device);
  virtual void OnDeviceRemoved(IDeviceDetails *device);
};
MAKE_AUDIOSINKDEVICE_FACTORY(SpeakerAudioSinkDevice, "speaker")

} // namespace media
} // namespace avatarflow

#endif //__SPEAKERAUDIOSINKDEVICE__