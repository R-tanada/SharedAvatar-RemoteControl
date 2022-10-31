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

#ifndef __RobotUIHelper__
#define __RobotUIHelper__

#include <string>

namespace avatarmobile
{
    class RobotUIHelper
    {
    private:
        int _batteryLevel;
        bool _charging;

    public:
        RobotUIHelper(/* args */);
        ~RobotUIHelper();

        std::string MoveRobotJSON(int forward, int rot);
        std::string ChangeNeckJSON(int neck);
        std::string ParseRobotJSON(const std::string &message);
        std::string AvatarIn();

        int GetBatteryLevel();
        bool IsCharging();
    };

} // namespace avatarmobile

#endif //__RobotUIHelper__
