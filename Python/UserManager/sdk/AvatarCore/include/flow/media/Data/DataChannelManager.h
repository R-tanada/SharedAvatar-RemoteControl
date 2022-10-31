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

#ifndef __DATACHANNELMANAGER__
#define __DATACHANNELMANAGER__

//#include <utils/ISingleton.h>

#include <utils/ListenerContainer.h>

#include "IDataChannel.h"
#include "IDataChannelManagerListener.h"

namespace avatarflow {
namespace media {
class SharedMemoryDataWriter;
class SharedMemoryDataGrabber;

class DataChannelManagerImpl;
class AvaFlow_API DataChannelManager
    : public utils::ListenerContainer<IDataChannelManagerListener *> {
private:
  DataChannelManagerImpl *_impl;

public:
  DECLARE_FIRE_METHOD(OnNewDataArrived,
                      (DataChannelManager * mngr, IDataChannel *channel,
                       const media::DataFrame *frame),
                      (mngr, channel, frame))

public:
  DataChannelManager(const std::string &name);
  virtual ~DataChannelManager();

  void Start();
  void Stop();

  void AttachReadMemory(media::SharedMemoryDataGrabber *grabber);
  void AttachWriteMemory(media::SharedMemoryDataWriter *writer);

  media::SharedMemoryDataGrabber *GetReadMemory();
  media::SharedMemoryDataWriter *GetWriteMemory();

  IDataChannelPtr CreateDataChannel(const utils::GUID &id);
  void RemoveDataChannel(const utils::GUID &id);
  IDataChannelPtr GetDataChannel(const utils::GUID &id);

  int WriteData(const utils::GUID &src, const char *data, int len);
};

//#define gDataChannelManager avatarflow::DataChannelManager::getInstance()

} // namespace media
} // namespace avatarflow

#endif //__DATACHANNELMANAGER__