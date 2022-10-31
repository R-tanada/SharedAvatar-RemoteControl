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

#ifndef __RTPSESSIONMANAGER__
#define __RTPSESSIONMANAGER__

#include <string>

#include "flow/media/RTP/RTPMediaChannel.h"

namespace avatarflow {
namespace services {

class RTPSenderService;
class RTPReceiverService;
class IServiceManager;

class RTPMetaData {
public:
  // alias for the created service
  std::string alias;

  // peerIP address to send data to, setting it to localIP 127.0.0.1 will
  // utilize cloud proxy service
  std::string peerIP;

  // port address to send or receive data from
  utils::uint port;

  // media type
  media::ERTPMediaType mediaType;

  // streamID, used by cloud proxy service
  utils::uint streamID;

  // resource name to use for sending or receiving
  std::string resourceName;

  // used by senders
  utils::uint quality;

  bool ParseJSON(JSONObject obj);
};

class RTPSessionManagerImpl;

class RTPSessionManager {
private:
  RTPSessionManagerImpl *_impl;

public:
  static const utils::GUID PATH_CreateSenderService;
  static const utils::GUID PATH_CreateReceiverService;

public:
  RTPSessionManager(/* args */);
  virtual ~RTPSessionManager();

  void Init(IServiceManager *manager);
  void Shutdown();

  RTPSenderService *CreateSenderService(const RTPMetaData *data);
  RTPReceiverService *CreateReceiverService(const RTPMetaData *data);
};

} // namespace services
} // namespace avatarflow

#endif //__RTPSESSIONMANAGER__