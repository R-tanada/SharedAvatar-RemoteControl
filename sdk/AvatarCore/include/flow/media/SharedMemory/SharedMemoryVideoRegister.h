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

#ifndef __SHAREDMEMORYVIDEOREGISTER__
#define __SHAREDMEMORYVIDEOREGISTER__

#include <utils/GUID.h>
#include <utils/ISingleton.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class SharedMemoryVideoRegisterListener;

class AvaFlow_API SharedMemoryVideoRegister
    : public utils::ISingleton<SharedMemoryVideoRegister> {
   private:
    std::list<utils::GUID> _sharedVideos;

    SharedMemoryVideoRegisterListener* _listener;

   public:
    SharedMemoryVideoRegister(/* args */);
    ~SharedMemoryVideoRegister();

    void RegisterSharedVideo(const utils::GUID& name);
    void RemoveSharedVideo(const utils::GUID& name);

    const std::list<utils::GUID>& GetSharedVideos();
};

}  // namespace media
}  // namespace avatarflow

#endif  //__SHAREDMEMORYVIDEOREGISTER__