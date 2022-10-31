

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

#ifndef __IRTPService__
#define __IRTPService__

#include "IService.h"
#include "flow/media/RTP/RTPMediaChannel.h"
#include "utils/IDelegate.h"

#undef StartService

namespace avatarflow {
namespace media {
class GstPipeline;
class RTPMediaChannel;
} // namespace media

namespace services {

class IRTPServiceImpl;

class AvaFlow_API IRTPService : public IService {
public:
  DECLARE_RTTI;
  DECLARE_PROPERTY_TYPE(PeerKey, std::string, );
  DECLARE_PROPERTY_TYPE(PeerIP, std::string, );
  DECLARE_PROPERTY_TYPE(PeerSSRC, utils::uint, );
  DECLARE_PROPERTY_TYPE(Secured, bool, );
  DECLARE_PROPERTY_TYPE(RTCP, bool, );
  DECLARE_PROPERTY_TYPE(FEC, bool, );

  DECLARE_PROPERTY_TYPE(LocalKey, std::string, );
  DECLARE_PROPERTY_TYPE(LocalSSRC, utils::uint, );

  DECLARE_SETGET_PROP(PeerKey, std::string);
  DECLARE_SETGET_PROP(PeerIP, std::string);
  DECLARE_SETGET_PROP(PeerSSRC, utils::uint);

  DECLARE_SETGET_PROP(LocalKey, std::string);
  DECLARE_SETGET_PROP(LocalSSRC, utils::uint);
  DECLARE_SETGET_PROP(Secured, bool);
  DECLARE_SETGET_PROP(RTCP, bool);
  DECLARE_SETGET_PROP(FEC, bool);

private:
protected:
  IRTPServiceImpl *_impl;

public:
  IRTPService(const std::string &type);
  virtual ~IRTPService();

  virtual media::RTPMediaChannel *
  AddRTPChannel(const media::RTPMediaChannel &channel) = 0;

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual void Update(float dt);
  virtual bool IsRunning();

  virtual void LoadSettings(const JSONObject &settings);
  virtual media::RTPMediaChannel *LoadChannel(const JSONObject &settings,
                                              media::ERTPMediaType media) = 0;

  virtual media::RTPMediaChannel *GetNamedChannel(media::ERTPMediaType media,
                                                  const std::string &name);

  virtual const std::vector<media::RTPMediaChannel *> &
  GetChannels(media::ERTPMediaType media);

  avatarflow::media::GstPipeline *GetPipeline();

  virtual std::string CreatePipelineString() = 0;
  virtual void LinkWithPipeline(avatarflow::media::GstPipeline *p) {}

  void PrintStats();

  virtual void OnSettingsUpdated(const JSONObject &settings);
};

} // namespace services
} // namespace avatarflow

#endif //__IRTPService__