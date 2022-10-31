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

#ifndef __NEWMEROBOTCAPS__
#define __NEWMEROBOTCAPS__

#include "RobotCapabilities.h"

namespace avatarflow {
namespace robot {

class NewMeRobotCaps : public RobotCapabilities {
   private:
    /* data */
   public:
    NewMeRobotCaps(/* args */);
    ~NewMeRobotCaps();
};

}  // namespace robot

}  // namespace avatarflow

#endif  //__NEWMEROBOTCAPS__