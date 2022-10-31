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

#ifndef __RTPReceiverService__
#define __RTPReceiverService__

#include "IRTPService.h"

namespace avatarflow {
namespace media {
class IVideoGrabber;
class IDataGrabber;
class IAudioGrabber;
class AudioAppSinkHandler;
class DataAppSinkHandler;
} // namespace media
namespace services {
class AvaFlow_API RTPReceiverService : public IRTPService {
  DECLARE_RTTI;

private:
protected:
public:
  RTPReceiverService(/* args */);
  virtual ~RTPReceiverService();

  virtual std::string CreatePipelineString();
  media::IVideoGrabber *GetVideoSink(const std::string &channelName);
  media::IAudioGrabber *GetAudioSink(const std::string &channelName);
  media::IDataGrabber *GetDataSink(const std::string &channelName);

  virtual void LinkWithPipeline(avatarflow::media::GstPipeline *p);
  virtual media::RTPMediaChannel *
  AddRTPChannel(const media::RTPMediaChannel &channel);
  virtual media::RTPMediaChannel *LoadChannel(const JSONObject &settings,
                                              media::ERTPMediaType media);
};
MAKE_SERVICE_FACTORY(RTPReceiverService, "RTPReceiver");

} // namespace services
} // namespace avatarflow

#endif //__RTPReceiverService__