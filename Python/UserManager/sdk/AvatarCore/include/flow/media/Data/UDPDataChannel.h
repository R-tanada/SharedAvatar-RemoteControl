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

#ifndef __UDPDATACHANNEL__
#define __UDPDATACHANNEL__

#include "IDataChannel.h"

namespace avatarflow {
namespace media {

class UDPDataChannelImpl;
class UDPDataChannel : public IDataChannel {
   private:
    UDPDataChannelImpl* _impl;

   public:
    UDPDataChannel(DataChannelManager* manager, const utils::GUID& id);
    ~UDPDataChannel();

    bool Init(utils::uint recvPort);
    virtual bool Close();

    virtual int Write(const char* data, int len);
    virtual int Read(char* data, int len);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__UDPDATACHANNEL__