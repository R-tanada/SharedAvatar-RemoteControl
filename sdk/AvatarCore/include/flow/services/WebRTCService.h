
#ifdef WEBRTC_SUPPORT

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

#ifndef __WEBRTCSERVICE__
#define __WEBRTCSERVICE__

#include "IService.h"
#include "media/WebRTC/WebRTCServer.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace services {
class WebRTCServiceImpl;
class WebRTCService : public IService {
public:
  DECLARE_RTTI;
  DECLARE_PROPERTY_TYPE(StunServer, std::string, );
  DECLARE_PROPERTY_TYPE(SignalingServer, std::string, );
  DECLARE_PROPERTY_TYPE(PeerID, uint, );

  DECLARE_SETGET_PROP(StunServer, std::string);
  DECLARE_SETGET_PROP(SignalingServer, std::string);
  DECLARE_SETGET_PROP(PeerID, uint);

private:
  WebRTCServiceImpl *_impl;

public:
  WebRTCService();
  virtual ~WebRTCService();

  media::WebRTCServer *GetServer();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(WebRTCService, "WebRTC");

} // namespace services
} // namespace avatarflow

#endif //__WEBRTCSERVICE__

#endif