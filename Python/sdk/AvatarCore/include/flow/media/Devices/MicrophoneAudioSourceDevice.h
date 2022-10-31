

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

#ifndef __MicrophoneAudioSOURCEDEVICE__
#define __MicrophoneAudioSOURCEDEVICE__

#include "DeviceMonitorManager.h"
#include "IAudioSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API MicrophoneAudioSourceDevice : public IAudioSourceDevice,
                                                public IDeviceMonitorListener {
  DECLARE_RTTI;

private:
  struct Settings {
    Settings()
        : _device(""), _samplerate(0), _channels(0), _targetChannel(-1),
          _format("") {}
    std::string _device;
    int _samplerate;
    int _channels;
    int _targetChannel;
    std::string _format;
  };
  void *_capsfilter;
  Settings _oldSettings, _settings;

  IDeviceDetails *_selectedDevice;

  DECLARE_PROPERTY_TYPE(Device, std::string, ); // Capture device
  DECLARE_PROPERTY_TYPE(SampleRate, int, );
  DECLARE_PROPERTY_TYPE(Channels, int, );
  DECLARE_PROPERTY_TYPE(TargetChannel, int, );
  DECLARE_PROPERTY_TYPE(Format, std::string, );

  DECLARE_SETGET_PROP(Device, std::string); // Capture device
  DECLARE_SETGET_PROP(SampleRate, int);
  DECLARE_SETGET_PROP(Channels, int);
  DECLARE_SETGET_PROP(TargetChannel, int);
  DECLARE_SETGET_PROP(Format, std::string);

  bool _deviceChanged(IDeviceDetails *device);

public:
  MicrophoneAudioSourceDevice(/* args */);
  ~MicrophoneAudioSourceDevice();

  virtual void Close();

  virtual std::string BuildPipeline(const std::string &name);
  virtual bool UpdateSettings();
  virtual void LinkWithPipeline(GstPipeline *pipeline);
  virtual void OnDeviceAdded(IDeviceDetails *device);
  virtual void OnDeviceRemoved(IDeviceDetails *device);
};

MAKE_AUDIOSOURCEDEVICE_FACTORY(MicrophoneAudioSourceDevice, "microphone")

} // namespace media
} // namespace avatarflow

#endif //__MicrophoneAudioSOURCEDEVICE__