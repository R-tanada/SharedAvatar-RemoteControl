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

#ifndef __MEDIADEVICEFACTORY__
#define __MEDIADEVICEFACTORY__

#include <utils/ISingleton.h>

#include <map>
#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class IVideoSourceDevice;
class IAudioSourceDevice;
class IAudioSinkDevice;
class IVideoSinkDevice;
class IAudioSinkDeviceFactory;
class IVideoSinkDeviceFactory;
class IDataSourceDevice;
class IDataSinkDevice;
class IAudioSourceDeviceFactory;
class IVideoSourceDeviceFactory;
class IDataSourceDeviceFactory;
class IDataSinkDeviceFactory;

class AvaFlow_API MediaDeviceFactory
    : public utils::ISingleton<MediaDeviceFactory> {
   private:
    /* data */
    std::map<std::string, IVideoSourceDeviceFactory *> _videoFactories;
    std::map<std::string, IAudioSourceDeviceFactory *> _audioFactories;
    std::map<std::string, IDataSourceDeviceFactory *> _dataFactories;

    std::map<std::string, IAudioSinkDeviceFactory *> _audioSinkFactories;
    std::map<std::string, IVideoSinkDeviceFactory *> _videoSinkFactories;
    std::map<std::string, IDataSinkDeviceFactory *> _dataSinkFactories;

   public:
    MediaDeviceFactory(/* args */);
    ~MediaDeviceFactory();

    void AddVideoSourceFactory(IVideoSourceDeviceFactory *f);
    void AddAudioSourceFactory(IAudioSourceDeviceFactory *f);
    void AddDataSourceFactory(IDataSourceDeviceFactory *f);

    void AddAudioSinkFactory(IAudioSinkDeviceFactory *f);
    void AddVideoSinkFactory(IVideoSinkDeviceFactory *f);
    void AddDataSinkFactory(IDataSinkDeviceFactory *f);

    IVideoSourceDevice *CreateVideoSource(const std::string &name);
    IAudioSourceDevice *CreateAudioSource(const std::string &name);
    IDataSourceDevice *CreateDataSource(const std::string &name);

    IAudioSinkDevice *CreateAudioSink(const std::string &name);
    IVideoSinkDevice *CreateVideoSink(const std::string &name);
    IDataSinkDevice *CreateDataSink(const std::string &name);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__MEDIADEVICEFACTORY__