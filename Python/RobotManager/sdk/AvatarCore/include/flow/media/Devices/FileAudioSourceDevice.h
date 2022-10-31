

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

#ifndef __FileAudioSOURCEDEVICE__
#define __FileAudioSOURCEDEVICE__

#include "IAudioSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API FileAudioSourceDevice : public IAudioSourceDevice {
    DECLARE_RTTI;

   private:
    std::string _path;

    DECLARE_PROPERTY_TYPE(Path, std::string, );  // file path

    DECLARE_SETGET_PROP(Path, std::string);  // file path

   public:
    FileAudioSourceDevice(/* args */);
    ~FileAudioSourceDevice();

    virtual std::string BuildPipeline(const std::string &name);
    virtual bool StopOnEOS() { return true; }
};

MAKE_AUDIOSOURCEDEVICE_FACTORY(FileAudioSourceDevice, "file")

}  // namespace media
}  // namespace avatarflow

#endif  //__FileAudioSOURCEDEVICE__