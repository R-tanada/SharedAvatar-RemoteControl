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

#ifndef __DEVICECAPS__
#define __DEVICECAPS__

#include <string>
#include <vector>

#include "flow/avatarflow_config.h"
#include "utils/point2D.h"

namespace avatarflow {
namespace media {

enum EDeviceType { Device_VideoSource, Device_AudioSource, Device_AudioSink };

/**
 * @brief Abstract class for the devices used in the system
 *
 */
class IDeviceCaps {
public:
  virtual ~IDeviceCaps() {}
  std::string mediatype;
  virtual void Parse(void *caps);

  virtual std::string ToString() { return mediatype; }
};

/**
 * @brief Contains an audio interface device capabilities
 *
 */
class AvaFlow_API AudioDeviceCaps : public IDeviceCaps {
public:
  std::vector<std::string> formats;
  int minChannels;
  int maxChannels;
  int maxRate;

  AudioDeviceCaps() : minChannels(0), maxChannels(0), maxRate(0) {}
  virtual void Parse(void *caps);

  virtual std::string ToString();
};

/**
 * @brief Contains a video capture interface (Camera) device capabilities
 *
 */
class AvaFlow_API VideoDeviceCaps : public IDeviceCaps {
public:
  std::vector<std::string> formats;
  int width;
  int height;
  std::vector<utils::vector2di> framerate;

public:
  VideoDeviceCaps() : width(0), height(0) {}
  virtual void Parse(void *caps);

  virtual std::string ToString();
};

class AvaFlow_API RequestedAudioCaps {
public:
  RequestedAudioCaps() : channels(2), rate(44100) {}
  std::string mediaType;
  std::string format;
  int channels;
  int rate;
  virtual std::string ToString();
};
class AvaFlow_API RequestedVideoCaps {
public:
  RequestedVideoCaps() : width(1280), height(720), framerate(30, 1) {}
  std::string mediaType;
  std::string format;
  int width;
  int height;
  utils::vector2di framerate;
  virtual std::string ToString();
};

class IDeviceDetailsImpl;
class AvaFlow_API IDeviceDetails {
protected:
  EDeviceType _type;

public:
  IDeviceDetailsImpl *_internals;
  std::string displayName;
  std::string elementName;
  std::string deviceID;
  std::vector<IDeviceCaps *> caps;

  IDeviceDetails(EDeviceType type);
  virtual ~IDeviceDetails();

  EDeviceType GetType() { return _type; }

  virtual bool Parse(void *device, bool &isdefault);
  virtual IDeviceCaps *CreateCaps() = 0;

  void *GetDevicePtr();
};
class AvaFlow_API IAudioDeviceDetails : public IDeviceDetails {
public:
  IAudioDeviceDetails(EDeviceType type) : IDeviceDetails(type) {}
  ~IAudioDeviceDetails() {}
  virtual IDeviceCaps *CreateCaps() { return new AudioDeviceCaps(); }
  bool GetClosestCaps(RequestedAudioCaps &caps);
};
class AvaFlow_API AudioSrcDeviceDetails : public IAudioDeviceDetails {
public:
  AudioSrcDeviceDetails() : IAudioDeviceDetails(Device_AudioSource) {}
  ~AudioSrcDeviceDetails() {}
  virtual bool Parse(void *device, bool &isdefault) {
    return IDeviceDetails::Parse(device, isdefault);
  }
};
class AvaFlow_API AudioSinkDeviceDetails : public IAudioDeviceDetails {
public:
  AudioSinkDeviceDetails() : IAudioDeviceDetails(Device_AudioSink) {}
  ~AudioSinkDeviceDetails() {}
  virtual bool Parse(void *device, bool &isdefault) {
    return IDeviceDetails::Parse(device, isdefault);
  }
};
class AvaFlow_API VideoSrcDeviceDetails : public IDeviceDetails {
public:
  VideoSrcDeviceDetails() : IDeviceDetails(Device_VideoSource) {}
  ~VideoSrcDeviceDetails() {}

  virtual bool Parse(void *device, bool &isdefault) {
    return IDeviceDetails::Parse(device, isdefault);
  }
  virtual IDeviceCaps *CreateCaps() { return new VideoDeviceCaps(); }
  bool GetClosestCaps(RequestedVideoCaps &caps);
};

} // namespace media
} // namespace avatarflow

#endif //__DEVICECAPS__