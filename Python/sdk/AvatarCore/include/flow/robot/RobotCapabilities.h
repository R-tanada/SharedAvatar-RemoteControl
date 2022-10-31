

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

#ifndef __ROBOTCAPABILITIES__
#define __ROBOTCAPABILITIES__

#include <utils/Capabilities.h>

namespace avatarflow {
namespace robot {
class RobotCapabilities : public utils::Capabilities {
   private:
   public:
    RobotCapabilities() {}
    ~RobotCapabilities() {}

   public:
    DECLARE_CAPABILITY_NAME(Name);
    DECLARE_CAPABILITY_NAME(SerialNumber);
    DECLARE_CAPABILITY_NAME(Model);

    DECLARE_CAPABILITY_NAME(MoveForward);
    DECLARE_CAPABILITY_NAME(ForwardSpeed);
    DECLARE_CAPABILITY_NAME(MoveBackward);
    DECLARE_CAPABILITY_NAME(BackwardSpeed);
    DECLARE_CAPABILITY_NAME(MoveSideways);
    DECLARE_CAPABILITY_NAME(SidewaysSpeed);
    DECLARE_CAPABILITY_NAME(RotationAxis);  // axises can rotate
    DECLARE_CAPABILITY_NAME(RotationSpeed);
    DECLARE_CAPABILITY_NAME(NeckDoF);  // Neck degrees of freedom
    DECLARE_CAPABILITY_NAME(BatteryCapacity);
    DECLARE_CAPABILITY_NAME(AdjustableHeight);
    DECLARE_CAPABILITY_NAME(LIDAR);
    DECLARE_CAPABILITY_NAME(LIDARAngleYaw);    // LIDAR Scanning Yaw Angle
    DECLARE_CAPABILITY_NAME(LIDARAnglePitch);  // LIDAR Scanning Pitch Angle
};

}  // namespace robot
}  // namespace avatarflow

#endif  //__ROBOTCAPABILITIES__