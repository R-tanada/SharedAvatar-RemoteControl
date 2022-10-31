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

#ifndef __UDPMediaSourceDevie__
#define __UDPMediaSourceDevie__

#include "IAudioSourceDevice.h"
#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {

class UDPMediaSourceDevie {
   public:
    std::string _socketName;
    utils::uint _srcPort;

    DECLARE_PROPERTY_TYPE(SocketName, std::string, );
    DECLARE_PROPERTY_TYPE(SourcePort, utils::uint, );

    DECLARE_SETGET_PROP(SocketName, std::string);
    DECLARE_SETGET_PROP(SourcePort, utils::uint);

    /* data */
   public:
    UDPMediaSourceDevie(/* args */);
    ~UDPMediaSourceDevie();

    virtual std::string BuildPipeline(const std::string &name);
};

class VideoUDPMediaSourceDevice : public IVideoSourceDevice,
                                  public UDPMediaSourceDevie {
    DECLARE_RTTI;

   private:
   public:
    VideoUDPMediaSourceDevice(/* args */);
    ~VideoUDPMediaSourceDevice();

    virtual std::string BuildPipeline(const std::string &name) {
        return UDPMediaSourceDevie::BuildPipeline(name);
    }
};

class AudioUDPMediaSourceDevice : public IAudioSourceDevice,
                                  public UDPMediaSourceDevie {
    DECLARE_RTTI;

   private:
   public:
    AudioUDPMediaSourceDevice(/* args */);
    ~AudioUDPMediaSourceDevice();

    virtual std::string BuildPipeline(const std::string &name) {
        return UDPMediaSourceDevie::BuildPipeline(name);
    }
};
class DataUDPMediaSourceDevice : public IAudioSourceDevice,
                                 public UDPMediaSourceDevie {
    DECLARE_RTTI;

   private:
   public:
    DataUDPMediaSourceDevice(/* args */);
    ~DataUDPMediaSourceDevice();

    virtual std::string BuildPipeline(const std::string &name) {
        return UDPMediaSourceDevie::BuildPipeline(name);
    }
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(VideoUDPMediaSourceDevice, "RTP");
MAKE_AUDIOSOURCEDEVICE_FACTORY(AudioUDPMediaSourceDevice, "RTP");
MAKE_AUDIOSOURCEDEVICE_FACTORY(DataUDPMediaSourceDevice, "RTP");
}  // namespace media
}  // namespace avatarflow

#endif  //__UDPMediaSourceDevie__