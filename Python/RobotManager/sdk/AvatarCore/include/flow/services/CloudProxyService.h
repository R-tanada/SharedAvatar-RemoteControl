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

#ifndef __CLOUDPROXYSERVICE__
#define __CLOUDPROXYSERVICE__

#include "IService.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {
class RTPMediaChannel;
}
namespace services {

class CloudProxyServiceImpl;

class AvaFlow_API CloudProxyService : public IService {
protected:
  friend class CloudProxyServiceImpl;

  bool NotifyJoinedSession(utils::uint peerID);
  bool NotifyClosedSession(utils::uint peerID);

public:
  /**
   * @brief <Published Topic> published when a forwarded message is sent to this
   * peer path: cloud/message/get data: JSON object contains message details
   *  {
   *    "Source":"Source Service Name",
   *    "targe":"Target Service Name",
   *    "path":"Message path",
   *    "data":"Payload data"
   *  }
   */
  static const utils::GUID PATH_MessageForwarded;
  // static const utils::GUID PATH_CloudReady;
  // static const utils::GUID PATH_CloudDisconnected;

  /**
   * @brief <Subscribed Topic> set the peerID for this connection
   * path: cloud/peer/set
   * data: Json object with contents:
   *  {
   *    "peerID":"Number"
   *  }
   */
  static const utils::GUID PATH_SetPeerID;
  static const utils::GUID PATH_PeerJoining;
  static const utils::GUID PATH_SessionReceived;
  static const utils::GUID PATH_PeerJoined;
  static const utils::GUID PATH_PeerDisconnected;
  static const utils::GUID PATH_CloudStatus;
  static const utils::GUID PATH_SessionCreated;
  static const utils::GUID PATH_RemoteSessionClosed;

  static const utils::GUID PATH_SubscribeStreams;
  static const utils::GUID PATH_PublishStreams;

  static const utils::GUID
      PATH_RequestStatus; // request the status of the cloud, will reply in a
                          // broadcast?
  static const utils::GUID PATH_JoinPeer;
  static const utils::GUID PATH_DisconnectPeer;
  static const utils::GUID PATH_PublishCloudMessage;
  static const utils::GUID PATH_CloudQOSReport;

  struct PublisherInfo {
    PublisherInfo() : channel(0) {}
    media::RTPMediaChannel *channel;
    int streamID;
    int portNumber;
    bool isReliable;
  };
  struct SubscriberInfo {
    utils::uint targetPeerID;
    utils::uint streamID;
    utils::uint portNumber;
  };

  void _PublishCloudStatus(const std::string &to = "");

private:
  CloudProxyServiceImpl *_impl;

public:
  DECLARE_RTTI

  DECLARE_PROPERTY_TYPE(PeerID, utils::uint, );
  DECLARE_PROPERTY_TYPE(CreateSession, bool, );
  DECLARE_PROPERTY_TYPE(Environment, std::string, );
  DECLARE_PROPERTY_TYPE(SessionID, std::string, );
  DECLARE_PROPERTY_TYPE(SessionType, std::string, );
  DECLARE_PROPERTY_TYPE(Region, std::string, );
  DECLARE_PROPERTY_TYPE(PingPongRetries, utils::uint, );
  DECLARE_PROPERTY_TYPE(PingPongTimeout, utils::uint, );

  DECLARE_SETGET_PROP(PeerID, utils::uint);
  DECLARE_SETGET_PROP(CreateSession, bool);
  DECLARE_SETGET_PROP(Environment, std::string);
  DECLARE_SETGET_PROP(SessionID, std::string);
  DECLARE_SETGET_PROP(SessionType, std::string);
  DECLARE_SETGET_PROP(Region, std::string);
  DECLARE_SETGET_PROP(PingPongRetries, utils::uint);
  DECLARE_SETGET_PROP(PingPongTimeout, utils::uint);

public:
  CloudProxyService(/* args */);
  virtual ~CloudProxyService();

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();
  virtual void Update(float dt);

  // get cloud proxy status
  bool IsAuthenticated();
  bool SessionCreated();
  bool PublishersAlive();
  bool SubscribersAlive();

  // Functions

  /**
   * @brief Send hello message to cloud proxy in order to authenticate with
   * the cloud service
   *
   * @return true
   * @return false
   */
  bool SendHello(utils::uint peerID);
  /**
   * @brief Close the connection and clean any resources at the cloud side
   *
   * @return true
   * @return false
   */
  bool Goodbye();

  /**
   * @brief Create a Session in the cloud, usually initated by the avatar
   *
   * @param uuid
   * @return std::string
   */
  std::string CreateSession(const std::string &uuid = "");

  /**
   * @brief Join the session as publisher, currently calling this will
   * override any publishers already joined by this peer
   *
   * @param sessionID
   * @param ifo
   * @return true
   * @return false
   */
  bool JoinSessionPublisher(
      const std::string &sessionID,
      const std::vector<CloudProxyService::PublisherInfo> &ifo);

  /**
   * @brief Join as subscriber inside the session
   *
   * @param sessionID
   * @param ifo
   * @return true
   * @return false
   */
  bool JoinSessionSubscriber(
      const std::string &sessionID,
      const std::vector<CloudProxyService::SubscriberInfo> &ifo);
  /**
   * @brief Automatically populate all RTP channels that publishes into the
   * cloud. Make sure the service is running in the same process as the other
   * RTP channels
   *
   */
  void JoinPublishers();
  /**
   * @brief Automatically populate all RTP channels that subscribes into the
   * cloud. Make sure the service is running in the same process as the other
   * RTP channels
   *
   */
  void JoinSubscribers(utils::uint targetPeerID);

  /**
   * @brief Exits an existing session
   */
  bool LeaveSession();

  /**
   * @brief Publishes a message to a peer even without being connected into
   * it, or being in the same session.
   *
   * @param peerID
   * @param message
   * @return true
   * @return false
   */
  bool SendMessageToPeer(utils::uint peerID, const std::string &message,
                         bool immediate = false);

  bool SendDisconnected(utils::uint peerID, bool immediate = false);
  bool RequestJoinPeer(utils::uint peerID);
  bool SendSessionID(utils::uint peerID);

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);

  static void PublishJoinSessionSubscriber(
      const utils::GUID &src, const std::string &sessionID,
      const std::vector<CloudProxyService::SubscriberInfo> &ifo);

  static void PublishJoinSessionPublisher(
      const utils::GUID &src, const std::string &sessionID,
      const std::vector<CloudProxyService::PublisherInfo> &ifo);
};
MAKE_SERVICE_FACTORY(CloudProxyService, "CloudProxy")

} // namespace services
} // namespace avatarflow

#endif //__CLOUDPROXYSERVICE__