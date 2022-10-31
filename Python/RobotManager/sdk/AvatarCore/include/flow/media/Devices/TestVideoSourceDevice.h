

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

#ifndef __TestVIDEOSOURCEDEVICE__
#define __TestVIDEOSOURCEDEVICE__

#include "IVideoSourceDevice.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {
class TestVideoSourceDevice : public IVideoSourceDevice {
    DECLARE_RTTI;

   private:
    int _cameraIndex;
    int _width;
    int _height;
    int _framerate;
    std::string _captureType;

    DECLARE_PROPERTY_TYPE(CameraIndex, int, );          // Capture width
    DECLARE_PROPERTY_TYPE(Width, int, );                // Capture width
    DECLARE_PROPERTY_TYPE(Height, int, );               // Capture height
    DECLARE_PROPERTY_TYPE(Framerate, int, );            // Capture height
    DECLARE_PROPERTY_TYPE(CaptureType, std::string, );  // Raw/JPEG

    DECLARE_SETGET_PROP(CameraIndex, int);
    DECLARE_SETGET_PROP(Width, int);
    DECLARE_SETGET_PROP(Height, int);
    DECLARE_SETGET_PROP(Framerate, int);
    DECLARE_SETGET_PROP(CaptureType, std::string);

   public:
    TestVideoSourceDevice(/* args */);
    ~TestVideoSourceDevice();

    virtual std::string BuildPipeline(const std::string &name);
};

MAKE_VIDEOSOURCEDEVICE_FACTORY(TestVideoSourceDevice, "test")

}  // namespace media
}  // namespace avatarflow

#endif  //__TestVIDEOSOURCEDEVICE__