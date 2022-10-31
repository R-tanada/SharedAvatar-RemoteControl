

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
#include <utils/OSType.h>

#ifndef __MOBILE__

#ifndef __ScreenCapVIDEOSOURCEDEVICE__
#define __ScreenCapVIDEOSOURCEDEVICE__

#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class AvaFlow_API ScreenCapVideoSourceDevice : public IVideoSourceDevice {
    DECLARE_RTTI;

   private:
    int _x;
    int _y;
    int _width;
    int _height;

    DECLARE_PROPERTY_TYPE(X, int, );       // Capture X
    DECLARE_PROPERTY_TYPE(Y, int, );       // Capture Y
    DECLARE_PROPERTY_TYPE(Width, int, );   // Capture width
    DECLARE_PROPERTY_TYPE(Height, int, );  // Capture height

    DECLARE_SETGET_PROP(X, int);
    DECLARE_SETGET_PROP(Y, int);
    DECLARE_SETGET_PROP(Width, int);
    DECLARE_SETGET_PROP(Height, int);

   public:
    ScreenCapVideoSourceDevice(/* args */);
    ~ScreenCapVideoSourceDevice();

    virtual std::string BuildPipeline(const std::string &name);
    virtual bool UpdateSettings();
    virtual void LinkWithPipeline(GstPipeline *pipeline);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(ScreenCapVideoSourceDevice, "ScreenCap")

}  // namespace media
}  // namespace avatarflow

#endif  //__ScreenCapVIDEOSOURCEDEVICE__
#endif