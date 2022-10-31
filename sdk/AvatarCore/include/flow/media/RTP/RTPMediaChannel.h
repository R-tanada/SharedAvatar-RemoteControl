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

#ifndef __RTPMEDIACHANNEL__
#define __RTPMEDIACHANNEL__

#include "flow/media/Builders/AudioReceiverBuilder.h"
#include "flow/media/Builders/AudioSenderBuilder.h"
#include "flow/media/Builders/DataReceiverBuilder.h"
#include "flow/media/Builders/DataSenderBuilder.h"
#include "flow/media/Builders/VideoReceiverBuilder.h"
#include "flow/media/Builders/VideoSenderBuilder.h"
#include "flow/media/Elements/PropertyInterpolator.h"

#include "utils/JSONUtils.h"
#include "utils/ListenerContainer.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class IVideoSourceDevice;
class IAudioSourceDevice;
class IDataSourceDevice;
class IAudioSinkDevice;
class IVideoSinkDevice;
class IDataSinkDevice;
class VideoAppSinkHandler;
class AudioAppSinkHandler;
class DataAppSinkHandler;

enum ERTPMediaType {
  RTPMedia_Video,
  RTPMedia_Audio,
  RTPMedia_Data,

  RTPMedia_Count
};
enum ERTPStreamType {
  RTPStream_Inbound,
  RTPStream_Outbound,

  RTPStream_Count
};

class QOSFeedback {
public:
  float lossRate;
  float avgLatency;
  unsigned long totalBytes;
};

class RTPMediaChannel;
class IRTPMediaChannelListener {
public:
  virtual ~IRTPMediaChannelListener() {}
  virtual void OnMediaChannelPortChanged(RTPMediaChannel *channel) {}
  virtual void OnMediaChannelOpen(RTPMediaChannel *channel) {}
  virtual void OnMediaChannelClose(RTPMediaChannel *channel) {}
};

class RTPMediaChannel
    : public utils::ListenerContainer<IRTPMediaChannelListener *> {
protected:
  DECLARE_FIRE_METHOD(OnMediaChannelPortChanged, (RTPMediaChannel * channel),
                      (channel))
  DECLARE_FIRE_METHOD(OnMediaChannelOpen, (RTPMediaChannel * channel),
                      (channel))
  DECLARE_FIRE_METHOD(OnMediaChannelClose, (RTPMediaChannel * channel),
                      (channel))
public:
  std::string name;
  utils::uint streamID;
  utils::uint rtpPort;
  utils::uint sendRtcpPort;
  utils::uint recvRtcpPort;

  bool isOpen;

  bool _autoSelectPort;

  media::GstPipeline *_pipeline;

  bool isDummy;

  bool isReliable;

  ERTPMediaType mediaType;
  ERTPStreamType streamType;

  void *rtpUDPChannel;
  void *fecElem;
  void *udpElem;

public:
  RTPMediaChannel(const RTPMediaChannel &o)
      : rtpPort(o.rtpPort), streamID(o.streamID), sendRtcpPort(o.sendRtcpPort),
        recvRtcpPort(o.recvRtcpPort), mediaType(o.mediaType),
        streamType(o.streamType), rtpUDPChannel(o.rtpUDPChannel),
        isDummy(o.isDummy), isReliable(o.isReliable), fecElem(o.fecElem),
        _autoSelectPort(o._autoSelectPort), _pipeline(o._pipeline),
        udpElem(o.udpElem), isOpen(o.isOpen) {}
  RTPMediaChannel(ERTPMediaType t, ERTPStreamType s)
      : rtpPort(0), _autoSelectPort(true), streamID(0), sendRtcpPort(9000),
        recvRtcpPort(9010), mediaType(t), streamType(s), rtpUDPChannel(0),
        isDummy(false), isReliable(false), fecElem(0), _pipeline(0), udpElem(0),
        isOpen(false) {}
  virtual ~RTPMediaChannel() {}
  virtual bool Parse(const JSONObject &stream);
  virtual void LinkWithPipeline(media::GstPipeline *pipeline, int idx);

  virtual void Update(float dt) {}

  virtual std::string GetElementName(int idx);

  bool MakeSurePortReady();
  virtual void Open();
  virtual void Close();

  virtual void SetPort(utils::uint port);

  virtual std::string BuildPipeline(int idx) = 0;

  virtual bool OnQOS(const QOSFeedback &qos) { return true; }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelSendVideoStreamImpl;
class RTPChannelSendVideoStream : public RTPMediaChannel {
public:
  RTPChannelSendVideoStreamImpl *_impl;

  RTPChannelSendVideoStream(const RTPMediaChannel &o);
  RTPChannelSendVideoStream();
  ~RTPChannelSendVideoStream();

  bool Parse(const JSONObject &stream);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);
  virtual std::string BuildPipeline(int idx);

  virtual void Update(float dt);
  virtual void Open();
  virtual void Close();

  virtual bool OnQOS(const QOSFeedback &qos);
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelReceiveVideoStreamImpl;
class RTPChannelReceiveVideoStream : public RTPMediaChannel {
public:
  RTPChannelReceiveVideoStreamImpl *_impl;
  media::VideoReceiverBuilder builder;

  RTPChannelReceiveVideoStream(const RTPMediaChannel &o);
  RTPChannelReceiveVideoStream();
  ~RTPChannelReceiveVideoStream();
  bool Parse(const JSONObject &stream);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);

  virtual std::string BuildPipeline(int idx);

  virtual void Update(float dt);
  virtual void Open();
  virtual void Close();
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelSendAudioStream : public RTPMediaChannel {
public:
  media::IAudioSourceDevice *captureDevice;
  media::AudioSenderBuilder builder;

  RTPChannelSendAudioStream(const RTPMediaChannel &o) : RTPMediaChannel(o) {}
  RTPChannelSendAudioStream();
  ~RTPChannelSendAudioStream();

  bool Parse(const JSONObject &stream);
  virtual std::string BuildPipeline(int idx);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);

  virtual void Open();
  virtual void Close();
};
//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelReceiveAudioStream : public RTPMediaChannel {
public:
  // media::IAppSinkHandler *appSink;
  // media::IAudioGrabber *grabber;
  media::AudioReceiverBuilder builder;

  media::IAudioSinkDevice *playDevice;
  /*
      media::SharedMemoryAudioWriter _shmemWriter;
      std::string Resource;
      int _size;*/

  RTPChannelReceiveAudioStream(const RTPMediaChannel &o)
      : RTPMediaChannel(o), playDevice(0) {}
  RTPChannelReceiveAudioStream();
  ~RTPChannelReceiveAudioStream();

  bool Parse(const JSONObject &stream);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);
  virtual std::string BuildPipeline(int idx);

  virtual void Open();
  virtual void Close();
};

//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelSendDataStream : public RTPMediaChannel {
public:
  media::IDataSourceDevice *captureDevice;
  media::DataSenderBuilder builder;

  RTPChannelSendDataStream(const RTPMediaChannel &o) : RTPMediaChannel(o) {}
  RTPChannelSendDataStream();
  ~RTPChannelSendDataStream();

  bool Parse(const JSONObject &stream);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);
  virtual std::string BuildPipeline(int idx);

  virtual void Open();
  virtual void Close();
};
//////////////////////////////////////////////////////////////////////////////////////////////////
class RTPChannelReceiveDataStream : public RTPMediaChannel {
public:
  media::DataReceiverBuilder builder;
  media::IDataSinkDevice *playDevice;

  RTPChannelReceiveDataStream(const RTPMediaChannel &o) : RTPMediaChannel(o) {}
  RTPChannelReceiveDataStream();
  ~RTPChannelReceiveDataStream();

  bool Parse(const JSONObject &stream);
  void LinkWithPipeline(media::GstPipeline *pipeline, int idx);
  virtual std::string BuildPipeline(int idx);

  virtual void Open();
  virtual void Close();
};

} // namespace media
} // namespace avatarflow

#endif //__RTPMEDIACHANNEL__
