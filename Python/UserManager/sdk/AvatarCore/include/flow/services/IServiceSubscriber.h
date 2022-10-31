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

#ifndef __ISERVICESUBSCRIBER__
#define __ISERVICESUBSCRIBER__

#include <utils/GUID.h>
#include <utils/atypes.h>
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace services {
class AvaFlow_API IServiceSubscriber {
   public:
    virtual ~IServiceSubscriber() {}
    virtual void Subscriber_OnMessage(const utils::GUID& serviceName,
                                      const utils::GUID& path,
                                      const std::string& data,
                                      utils::uint address, bool forwarded) {}
    virtual void Subscriber_OnData(const utils::GUID& serviceName,
                                   const utils::GUID& path, const void* data,
                                   utils::uint length, utils::uint address,
                                   bool forwarded) {}
};
}  // namespace services
}  // namespace avatarflow

#endif  //__ISERVICESUBSCRIBER__