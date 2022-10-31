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

#ifndef __ROBOTPROTOCOL__
#define __ROBOTPROTOCOL__

#include <utils/ListenerContainer.h>

#include "RobotControlMessage.h"

namespace avatarflow {
namespace media {
class IDataChannel;
}

namespace robot {

class RobotProtocolImpl;
class RobotProtocol;

class AvaFlow_API RobotProtocol {
   protected:
    friend class RobotProtocolImpl;
    RobotProtocolImpl* _impl;

   public:
    virtual void OnMessageArrived(const RobotControlMessage& msg) {}

   public:
    RobotProtocol(/* args */);
    ~RobotProtocol();

    void AttachChannel(media::IDataChannel* channel);

    void Send(RobotControlMessage& msg);
    RobotControlMessage* GetLastMessage(RobotMessage msg);
};

}  // namespace robot
}  // namespace avatarflow

#endif  //__ROBOTPROTOCOL__