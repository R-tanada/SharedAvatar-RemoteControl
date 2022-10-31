
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

#ifndef __VideoDevice_DShow__
#define __VideoDevice_DShow__

#include "IVideoDeviceHelper.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API VideoDevice_DShow : public IVideoDeviceHelper {
   private:
    /* data */
   public:
    VideoDevice_DShow(/* args */) {}
    ~VideoDevice_DShow() {}

    int GetDeviceCount();
};

}  // namespace media
}  // namespace avatarflow

#endif  //__VideoDevice_DShow__
#endif