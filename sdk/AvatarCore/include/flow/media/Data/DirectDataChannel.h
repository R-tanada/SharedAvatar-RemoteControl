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

#ifndef __DIRECTDATACHANNEL__
#define __DIRECTDATACHANNEL__

#include "IDataChannel.h"

namespace avatarflow {
namespace media {

class DirectDataChannelImpl;
class DirectDataChannel : public IDataChannel {
   private:
    DirectDataChannelImpl* _impl;

   public:
    DirectDataChannel(DataChannelManager* manager, const utils::GUID& id);
    ~DirectDataChannel();

    bool Init();
    virtual bool Close();
    virtual void NewDataArrived(const char* data, int len);

    virtual int Write(const char* data, int len);

    virtual bool GrabFrame();
    virtual bool HasNewFrame() const;
    virtual utils::uint GetFrameLength() const;
    virtual const media::DataFrame* GetLastFrame() const;
};

}  // namespace media
}  // namespace avatarflow

#endif  //__DIRECTDATACHANNEL__