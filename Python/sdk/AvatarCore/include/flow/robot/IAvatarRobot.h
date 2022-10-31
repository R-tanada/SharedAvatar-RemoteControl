

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

#ifndef __IAVATARROBOT__
#define __IAVATARROBOT__

#include <string>

namespace avatarflow {
namespace robot {

class RobotCapabilities;

// manages the communication with the robot hardware
class IAvatarRobot {
   private:
   public:
    IAvatarRobot() {}
    ~IAvatarRobot() {}

    virtual const RobotCapabilities *GetCapabilities() = 0;

    virtual void Set(const std::string &command, const std::string &args) = 0;
    virtual std::string Get(const std::string &command) = 0;
};

}  // namespace robot

}  // namespace avatarflow

#endif  //__IAVATARROBOT__