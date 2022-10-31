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

#ifndef __ROBOTMESSAGES__
#define __ROBOTMESSAGES__

#include <utils/GUID.h>

#include "flow/avatarflow_config.h"

#define DECLARE_ROBOT_MESSAGE(MSG) static const utils::GUID MSG_##MSG;
#define IMPLEMENT_ROBOT_MESSAGE(class, MSG) \
    const utils::GUID class ::MSG_##MSG(#MSG);

namespace avatarflow {
namespace robot {

typedef utils::GUID RobotMessage;

class AvaFlow_API RobotMessages {
   private:
    /* data */
   public:
    DECLARE_ROBOT_MESSAGE(Unkown);
    DECLARE_ROBOT_MESSAGE(Start);
    DECLARE_ROBOT_MESSAGE(Stop);
    DECLARE_ROBOT_MESSAGE(ForceStop);
    DECLARE_ROBOT_MESSAGE(Joints);
    DECLARE_ROBOT_MESSAGE(BatteryLevel);
    DECLARE_ROBOT_MESSAGE(LIDARData);
    DECLARE_ROBOT_MESSAGE(MoveDir);     // move as a vector
    DECLARE_ROBOT_MESSAGE(MoveWheels);  // individual wheel control
    DECLARE_ROBOT_MESSAGE(NeckAngle);
    DECLARE_ROBOT_MESSAGE(Parameters);  // general purpose parameters
};

}  // namespace robot
}  // namespace avatarflow

#endif  //__ROBOTMESSAGES__