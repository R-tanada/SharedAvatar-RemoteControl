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

#ifndef __IDATACHANNEL__
#define __IDATACHANNEL__

#include <utils/GUID.h>
#include <utils/ListenerContainer.h>
#include <utils/base.h>

#include "../AppSrc/IDataGrabber.h"

namespace avatarflow {
namespace media {
class IDataChannel;
class DataChannelManager;
class IDataChannelListener {
public:
  virtual ~IDataChannelListener() {}
  virtual void DataChannel_OnDataArrived(IDataChannel *channel,
                                         const char *data, int len) {}
};

class IDataChannel : public media::IDataGrabber,
                     public utils::ListenerContainer<IDataChannelListener *> {
protected:
  DECLARE_FIRE_METHOD(DataChannel_OnDataArrived,
                      (IDataChannel * channel, const char *data, int len),
                      (channel, data, len));

protected:
  utils::GUID _id;
  DataChannelManager *_manager;

public:
  IDataChannel(DataChannelManager *manager, const utils::GUID &id)
      : _id(id), _manager(manager) {}
  virtual ~IDataChannel() {}

  const utils::GUID &GetChannelID() { return _id; }

  virtual bool Close() = 0;

  virtual int Write(const char *data, int len) = 0;

  virtual void NewDataArrived(const char *data, int len) {
    FIRE_LISTENR_METHOD(DataChannel_OnDataArrived, (this, data, len));
  }

  int WriteString(const std::string &str);
};

MakeSharedPtr(IDataChannel)

} // namespace media
} // namespace avatarflow

#endif //__IDATACHANNEL__