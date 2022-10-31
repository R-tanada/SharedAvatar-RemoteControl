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

#ifndef __RTPSENDERSERVICE__
#define __RTPSENDERSERVICE__

#include "IRTPService.h"

namespace avatarflow {
namespace media {
class IAppSrcHandler;
} // namespace media
namespace services {
class AvaFlow_API RTPSenderService : public IRTPService {
  DECLARE_RTTI;

private:
  static const utils::GUID PATH_BitRateUpdate;

protected:
public:
  RTPSenderService(/* args */);
  virtual ~RTPSenderService();

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual void Update(float dt);

  virtual std::string CreatePipelineString();

  virtual media::RTPMediaChannel *
  AddRTPChannel(const media::RTPMediaChannel &channel);
  virtual media::RTPMediaChannel *LoadChannel(const JSONObject &settings,
                                              media::ERTPMediaType media);

  // virtual void LinkWithPipeline(avatarflow::media::GstPipeline *p);

  static bool SendBitrateChange(const utils::GUID &sender,
                                const utils::GUID &target, int bitrate,
                                int minQuant, int maxQuant);

  void Subscriber_OnMessage(const utils::GUID &serviceName,
                            const utils::GUID &path, const std::string &data,
                            utils::uint address, bool forwarded);
};
MAKE_SERVICE_FACTORY(RTPSenderService, "RTPSender");

} // namespace services
} // namespace avatarflow

#endif //__RTPSENDERSERVICE__