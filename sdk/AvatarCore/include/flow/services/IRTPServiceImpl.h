

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

#ifndef __IRTPSERVICEIMPL__
#define __IRTPSERVICEIMPL__

#include "IRTPService.h"
#include "flow/media/GStreamer/GstPipelineHandler.h"
#include "flow/media/RTP/RTPMediaChannel.h"

namespace avatarflow {
namespace media {
// class GstPipelineHandler;
}
namespace services {

class AvaFlow_API IRTPServiceImpl : public media::IPipelineListener {
private:
  /* data */

protected:
  virtual void OnPipelinePlaying(media::GstPipelineHandler *p);
  virtual void OnPipelineStopped(media::GstPipelineHandler *p);
  virtual void OnPipelineError(media::GstPipelineHandler *p);

public:
  enum RTPType { RTP_stream, RTP_securedStream, RTP_bin, RTP_securedbin };

  typedef std::vector<media::RTPMediaChannel *> RTPChannelVector;
  RTPChannelVector _channels[media::RTPMedia_Count]; //[RTPStream_Count];

  std::string _peerkey;
  std::string _peerIP;
  utils::uint _peerSSRC;

  std::string _localkey;
  utils::uint _localSSRC;
  bool _secured;
  bool _rtcp;
  bool _fec;

  RTPType _type;

  media::GstPipelineHandler *_handler;
  media::GstPipeline *_pipeline;
  // media::IAppSinkHandler *_sink;

  IRTPService *_owner;

  void *rtpbinElem;
  void *rtpsessionElem;

public:
  IRTPServiceImpl(IRTPService *o);
  ~IRTPServiceImpl();

  bool CreatePipeline();
  void DestroyPipeline();

  void PrintStats();
};

} // namespace services
} // namespace avatarflow

#endif //__IRTPSERVICEIMPL__