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

#ifndef __SERVICECOORDINATOR__
#define __SERVICECOORDINATOR__

#include <utils/ISingleton.h>

#include "IServiceSubscriber.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace services {
class ServiceCoordinatorImpl;

class AvaFlow_API ServiceCoordinator
    : public utils::ISingleton<ServiceCoordinator> {
private:
  ServiceCoordinatorImpl *_impl;

public:
  ServiceCoordinator(/* args */);
  ~ServiceCoordinator();

  /**
   * @brief Create a master service coordinator on the specified port
   *
   * @param name Name of the coordinator process
   * @param port Port to open for the service coordinator
   * @return true If the coordinator was created sucessfully
   * @return false If it fails to create on the specified port.
   */
  bool CreateCoordinator(const utils::GUID &name, int port = 0);

  /**
   * @brief Opens a connection to a service coordinator on the specified ip:port
   *
   * @param name Name of the coordinator process
   * @param ip IP Address of the coordinator, usually local host
   * @param port Port to connect to
   * @return true On success to open connection
   * @return false On Failure to establish a connection
   */
  bool OpenCoordinator(const utils::GUID &name,
                       const std::string &ip = "127.0.0.1",
                       int port = 0); // called by general services

  /**
   * @brief Check if this process is a master service coordinator
   *
   * @return true
   * @return false
   */
  bool IsMasterProcess();

  /**
   * @brief Close and clean the coordinator
   *
   */
  void Close();

  /**
   * @brief Get the Port the coordinator is using
   *
   * @return int
   */
  int GetPort();

  void Publish(const utils::GUID &srcServiceName,
               const utils::GUID &targetServiceName, const utils::GUID &path,
               const std::string &data);
  void Publish(const utils::GUID &srcServiceName,
               const utils::GUID &targetServiceName, const utils::GUID &path,
               const void *data, utils::uint length);

  void RegisterForwarder(const utils::GUID &srcServiceName,
                         const utils::GUID &path, IServiceSubscriber *sub);
  void RemoveForwarder(const utils::GUID &srcServiceName,
                       const utils::GUID &path, IServiceSubscriber *sub);

  void Subscribe(const utils::GUID &serviceName, const utils::GUID &path,
                 IServiceSubscriber *sub);
  void Unsubscribe(const utils::GUID &path, IServiceSubscriber *sub);
  void Unsubscribe(IServiceSubscriber *sub);

public:
  static void RequestOnlineServices(const std::string &replyTo);
  static void RegisterOnlineService(const utils::GUID &type,
                                    const std::string &alias);
  static void UnregisterOnlineService(const utils::GUID &type,
                                      const std::string &alias);
};

#define gServiceCoordinator                                                    \
  avatarflow::services::ServiceCoordinator::getInstance()

} // namespace services

} // namespace avatarflow

#endif //__SERVICECOORDINATOR__