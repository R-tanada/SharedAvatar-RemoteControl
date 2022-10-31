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

#ifndef __IVIDEODEVICEHELPER__
#define __IVIDEODEVICEHELPER__

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API IVideoDeviceHelper {
   public:
    virtual int GetDeviceCount() = 0;

    static IVideoDeviceHelper* CreateHelper();
};
}  // namespace media
}  // namespace avatarflow

#endif  //__IVIDEODEVICEHELPER__