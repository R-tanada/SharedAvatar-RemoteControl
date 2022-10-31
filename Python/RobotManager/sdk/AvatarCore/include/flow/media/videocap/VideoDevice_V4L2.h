
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

#ifndef __VIDEODEVICE_V4L2__
#define __VIDEODEVICE_V4L2__

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API VideoDevice_V4L2 {
   private:
    /* data */
   public:
    VideoDevice_V4L2(/* args */) {}
    ~VideoDevice_V4L2() {}

    static int GetCamerasCount();
};

}  // namespace media
}  // namespace avatarflow

#endif  //__VIDEODEVICE_V4L2__
#endif