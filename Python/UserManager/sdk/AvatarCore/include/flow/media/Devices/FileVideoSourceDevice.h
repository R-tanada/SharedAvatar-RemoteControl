

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

#ifndef __FileVideoSourceDevice__
#define __FileVideoSourceDevice__

#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API FileVideoSourceDevice : public IVideoSourceDevice {
    DECLARE_RTTI;

   private:
    std::string _path;

    DECLARE_PROPERTY_TYPE(Path, std::string, );  // file path

    DECLARE_SETGET_PROP(Path, std::string);  // file path

   public:
    FileVideoSourceDevice(/* args */);
    ~FileVideoSourceDevice();

    virtual std::string BuildPipeline(const std::string &name);
    virtual bool StopOnEOS() { return true; }
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(FileVideoSourceDevice, "file")

}  // namespace media
}  // namespace avatarflow

#endif  //__FileVideoSourceDevice__