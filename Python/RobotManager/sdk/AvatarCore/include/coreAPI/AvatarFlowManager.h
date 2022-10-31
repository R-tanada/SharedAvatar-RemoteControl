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

#ifndef __AVATARFLOWMANAGER__
#define __AVATARFLOWMANAGER__

#include <utils/ISingleton.h>

#include <string>

#include "AppLogDevice.h"
#include "avatarcore_config.h"

namespace avatarflow {

class AvatarFlowManagerImpl;

enum SessionType { Local, Cloud };
/**
 * @brief High level manager that initializes avatar core SDK and all required
 * managers.
 *
 */
class AvaCore_API AvatarFlowManager
    : public utils::ISingleton<AvatarFlowManager> {
public:
private:
  AvatarFlowManagerImpl *_impl;

public:
  AvatarFlowManager(/* args */);
  ~AvatarFlowManager();

  /**
   * @brief Starts avatar flow manager
   *
   * @param userName
   * @param password
   * @param session
   * @param master is this process a master process? There should be only one
   * master process per coordPort
   * @param appPath where the application and data path should be? In OSX, it
   * may be in ../Resources/
   * @param coordPort at which port the core should be linked at, there can be
   * only one core per port
   * @param appName name of the app
   * @param logPath location to save the logs into
   * @param logCB log function callback
   * @return true
   * @return false
   */
  bool Init(const std::string &userName, const std::string &password,
            SessionType session = SessionType::Cloud, bool master = false,
            const std::string &appPath = "", int coordPort = 40933,
            const std::string &appName = "", const std::string &logPath = ".",
            utils::AppLogDevice::LoggerFunction logCB = 0);

  /**
   * @brief Launch service subprocess (will be idle though)
   *
   * @param service
   */
  void LaunchService(const std::string &service);
  /**
   * @brief Terminates service subprocess
   *
   * @param service
   */
  void TerminateService(const std::string &service);
  /**
   * @brief Terminate all service sub processes
   *
   */
  void TerminateAllService();

  /**
   * @brief Starts service (hot start) assuming the owner service process
   * already launched
   *
   * @param service: Name of the services to be started, e.g.
   * "video/Capture". Wildcart is supported
   */
  void RequestStartService(const std::string &service);

  /**
   * @brief Stops service (hot start) assuming the owner service process
   * already launched
   *
   * @param service: Name of the services to be stopped, e.g.
   * "video/Capture". Wildcart is supported
   */
  void RequestStopService(const std::string &service);

  /**
   * @brief Shuts down Avatar Flow manager and any created resources
   *
   * @return true
   * @return false
   */
  bool Shutdown();

  // Retrives device ID of this hardware unit, mainly used with AvatarCore
  // hardware
  const std::string &GetDeviceID();

  // Get IP Address of an interface
  std::string GetIPAddress(const std::string &iface);

  // Get MAC Address of an interface
  std::string GetMACAddress(const std::string &iface);

  // Get version of a particular SDK
  // core: avatarCore SDK
  // stream: StreamSDK
  static std::string GetSDKVersion(const std::string &sdk);
};

#define gAvatarFlowManager avatarflow::AvatarFlowManager::getInstance()

} // namespace avatarflow

#endif //__AVATARFLOWMANAGER__