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

#ifndef __AUDIOVolumeELEMENT__
#define __AUDIOVolumeELEMENT__

#include <string>

#include "flow/avatarflow_config.h"
#include "utils/GUID.h"

namespace avatarflow {
namespace services {
class IServiceSubscriber;
}
namespace media {

class GstPipeline;

class AvaFlow_API AudioVolumeElement {
   private:
    void* _elem;
    std::string _name;
    GstPipeline* _pipeline;

   public:
    static const utils::GUID PATH_SetVolume;
    static const utils::GUID PATH_SetMute;

    float _volume;
    bool _mute;

   public:
    AudioVolumeElement(/* args */);
    virtual ~AudioVolumeElement();

    void SetVolume(float vol);
    float GetVolume();
    void Mute(bool m);
    bool IsMute();

    void Register(const utils::GUID& name, services::IServiceSubscriber* s);
    void UnRegister(services::IServiceSubscriber* s);

    std::string BuildPipeline(const std::string& prefix);

    virtual void LinkWithPipeline(GstPipeline* pipeline);

    virtual bool ProcessMessage(const utils::GUID& path,
                                const std::string& data);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__AUDIOVolumeELEMENT__