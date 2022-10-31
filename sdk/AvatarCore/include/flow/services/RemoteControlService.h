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

#ifndef __REMOTECONTROLSERVICE__
#define __REMOTECONTROLSERVICE__

#include "IService.h"

namespace avatarflow {
namespace services {
class RemoteControlServiceImpl;
class AvaFlow_API RemoteControlService : public IService {
   public:
    DECLARE_RTTI;

    static const utils::GUID PATH_InvokeRemoteMessage;

   protected:
    RemoteControlServiceImpl *_impl;

    DECLARE_PROPERTY_TYPE(Port, utils::uint, );

    DECLARE_SETGET_PROP(Port, utils::uint);

   public:
    RemoteControlService(/* args */);
    virtual ~RemoteControlService();

    virtual bool Init(ServiceContext *context);
    virtual bool StartService();
    virtual bool StopService();
    virtual bool IsRunning();
    virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                      const utils::GUID &path,
                                      const std::string &data,
                                      utils::uint address, bool forwarded);

    static void InvokeRemoteMessage(const utils::GUID &serviceName,
                                    const std::string &ip,
                                    const std::string &message);
};
MAKE_SERVICE_FACTORY(RemoteControlService, "RemoteControl");
}  // namespace services
}  // namespace avatarflow

#endif  //__REMOTECONTROLSERVICE__