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

#ifdef WEBRTC_SUPPORT

#ifndef __WEBRTCSERVER__
#define __WEBRTCSERVER__

#include <utils/ListenerContainer.h>
#include <utils/atypes.h>

#include <string>

using namespace utils;
namespace avatarflow {
namespace media {

class WebRTCServer;
class GstPipeline;

enum WebRTCErrorCodes {
    WebRTC_Ok,  // no error

    WebRTC_EConnectFinish,
    WebRTC_ERegisterServer,
    WebRTC_EHello,      // HELLO while not registering
    WebRTC_ESetupCall,  // setting up call
    WebRTC_ESessionOk,  // SESSION_OK while not calling error
    WebRTC_EPeerConnecting,
    WebRTC_EServerRegistering,
    WebRTC_EServerConnecting,
    WebRTC_ECallNegotiating

};

class IWebRTCServerListener {
   public:
    virtual void WebRTC_OnServerConnected(WebRTCServer *server) {}
    virtual void WebRTC_OnServerClosed(WebRTCServer *server) {}
    virtual void WebRTC_OnPipelineReady(WebRTCServer *server,
                                        media::GstPipeline *p) {}
    virtual void WebRTC_OnError(WebRTCServer *server, WebRTCErrorCodes code) {}
    virtual void WebRTC_OnChannelOpen(WebRTCServer *server,
                                      const std::string &channel) {}
    virtual void WebRTC_OnMessage(WebRTCServer *server,
                                  const std::string &channel,
                                  const std::string &msg) {}
    virtual void WebRTC_OnData(WebRTCServer *server, const std::string &channel,
                               utils::byte *data, int len) {}
    virtual void WebRTC_OnChannelClosed(WebRTCServer *server,
                                        const std::string &channel) {}
    virtual void WebRTC_OnChannelError(WebRTCServer *server,
                                       const std::string &channel, int code) {}

    // adding new media streams
    virtual void WebRTC_OnNewMediaStreamVideo(WebRTCServer *server,
                                              GstPipeline *pipeline,
                                              void *sink) {}
    virtual void WebRTC_OnNewMediaStreamAudio(WebRTCServer *server,
                                              GstPipeline *pipeline,
                                              void *sink) {}
};

class WebRTCServer : public utils::ListenerContainer<IWebRTCServerListener *> {
   protected:
    class WebRTCServerImpl;
    WebRTCServerImpl *_impl;

    DECLARE_FIRE_METHOD(WebRTC_OnServerConnected, (WebRTCServer * server),
                        (server));
    DECLARE_FIRE_METHOD(WebRTC_OnServerClosed, (WebRTCServer * server),
                        (server));
    DECLARE_FIRE_METHOD(WebRTC_OnPipelineReady,
                        (WebRTCServer * server, media::GstPipeline *p),
                        (server, p));
    DECLARE_FIRE_METHOD(WebRTC_OnError,
                        (WebRTCServer * server, WebRTCErrorCodes code),
                        (server, code));
    DECLARE_FIRE_METHOD(WebRTC_OnChannelOpen,
                        (WebRTCServer * server, const std::string &channel),
                        (server, channel));
    DECLARE_FIRE_METHOD(WebRTC_OnMessage,
                        (WebRTCServer * server, const std::string &channel,
                         const std::string &msg),
                        (server, channel, msg));
    DECLARE_FIRE_METHOD(WebRTC_OnData,
                        (WebRTCServer * server, const std::string &channel,
                         byte *data, int len),
                        (server, channel, data, len));
    DECLARE_FIRE_METHOD(WebRTC_OnChannelClosed,
                        (WebRTCServer * server, const std::string &channel),
                        (server, channel));
    DECLARE_FIRE_METHOD(WebRTC_OnChannelError,
                        (WebRTCServer * server, const std::string &channel,
                         int code),
                        (server, channel, code));

    DECLARE_FIRE_METHOD(WebRTC_OnNewMediaStreamVideo,
                        (WebRTCServer * server, GstPipeline *pipeline,
                         void *sink),
                        (server, pipeline, sink));
    DECLARE_FIRE_METHOD(WebRTC_OnNewMediaStreamAudio,
                        (WebRTCServer * server, GstPipeline *pipeline,
                         void *sink),
                        (server, pipeline, sink));

   public:
    WebRTCServer(/* args */);
    ~WebRTCServer();

    bool AttachPipeline(
        GstPipeline *pipeline,
        const std::string &binName);  // pipeline should have a webrtcbin with
                                      // name "sendrecv_rtc"
    bool ConnectAsync(
        const std::string &server, uint peer_id,
        const std::string &stunServer = "stun://stun.l.google.com:19302");

    bool CreateDataChannel(const std::string &name);

    bool SendString(const std::string &channelName, const std::string &text);
    bool SendBytes(const std::string &channelName, const utils::byte *data,
                   int len);
    void Close();
};

}  // namespace media

}  // namespace avatarflow

#endif  //__WEBRTCSERVER__

#endif