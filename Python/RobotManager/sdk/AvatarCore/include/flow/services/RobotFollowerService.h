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

#ifndef __ROBOTFOLLOWERSERVICE__
#define __ROBOTFOLLOWERSERVICE__

#include "IService.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace services {

class RobotFollowerServiceImpl;

class AvaFlow_API RobotFollowerService : public IService {
   public:
    DECLARE_RTTI
    static const utils::GUID PATH_ActivateFollower;

   private:
    /* data */
    RobotFollowerServiceImpl *_impl;

   public:
    RobotFollowerService(/* args */);
    virtual ~RobotFollowerService();

    virtual bool StartService();
    virtual bool StopService();
    virtual bool IsRunning();

    bool IsFollowerActive();
    void SetFollowerActive(bool active);

    virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                      const utils::GUID &path,
                                      const std::string &data,
                                      utils::uint address, bool forwarded);
};
MAKE_SERVICE_FACTORY(RobotFollowerService, "RobotFollower");

}  // namespace services
}  // namespace avatarflow
#endif  //__ROBOTFOLLOWERSERVICE__