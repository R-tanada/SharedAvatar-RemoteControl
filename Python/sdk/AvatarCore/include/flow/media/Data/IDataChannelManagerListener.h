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

#ifndef __IDATACHANNELMANAGERLISTENER__
#define __IDATACHANNELMANAGERLISTENER__

namespace avatarflow {
namespace media {

class DataChannelManager;
class IDataChannel;
class DataFrame;

class IDataChannelManagerListener {
public:
  virtual ~IDataChannelManagerListener() {}
  virtual void OnNewDataArrived(DataChannelManager *mngr, IDataChannel *channel,
                                const DataFrame *frame) {}
};

} // namespace media
} // namespace avatarflow

#endif //__IDATACHANNELMANAGERLISTENER__