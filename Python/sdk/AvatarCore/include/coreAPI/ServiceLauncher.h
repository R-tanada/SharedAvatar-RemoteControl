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

#ifndef __SERVICELAUNCHER__
#define __SERVICELAUNCHER__

#include <utils/GUID.h>

#include <string>

#include "avatarcore_config.h"

namespace avatarflow {
namespace services {

class ServiceLauncherImpl;
/**
 * @brief a class helper that manages the service processes running in the
 * background. It also handles restarting them when they crash.
 */
class AvaCore_API ServiceLauncher {
private:
  ServiceLauncherImpl *_impl;

public:
  static const utils::GUID PATH_ServiceReloaded;
  static const utils::GUID PATH_ServiceTerminated;
  static const utils::GUID PATH_ServiceLaunched;

public:
  ServiceLauncher(/* args */);
  ~ServiceLauncher();

  /**
   * @brief Starts service monitor, which tracks the status of the loaded
   * service process
   *
   */
  void StartMonitor();

  /**
   * @brief Stops service monitor
   *
   */
  void StopMonitor();

  /**
   * @brief Start up a service file (json or bin), you will need to supply the
   * name of the service to load it not its path, for example: if service name
   * is "Capture", then it will assume the json or bin file path is:
   * services/Capture-services.json
   *
   * @param service name of the service file to be loaded
   * @return true
   * @return false
   */
  bool LaunchServices(const std::string &service);

  /**
   * @brief Closes the preloaded service process
   *
   * @param service
   * @return true
   * @return false
   */
  bool StopServices(const std::string &service);

  /**
   * @brief Destroy all loaded services
   *
   * @return true
   * @return false
   */
  bool StopAllServices();
};

} // namespace services
} // namespace avatarflow

#endif //__SERVICELAUNCHER__