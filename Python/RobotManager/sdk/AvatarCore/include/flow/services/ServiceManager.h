
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

#ifndef __SERVICEMANAGER__
#define __SERVICEMANAGER__

#include "IService.h"
#include "IServiceManager.h"
#include "IServiceSubscriber.h"
#include "flow/avatarflow_config.h"

#include <mutex>

using namespace utils;

#undef StartService

namespace avatarflow {
namespace services {
/**
    Implementation of the IServiceManager. Provides the functionality to
    load services from json file, and the management to start/stop the services.

    @author Yamen Saraiji
*/
class AvaFlow_API ServiceManager : public IServiceManager,
                                   public IServiceSubscriber {
public:
  static const utils::GUID PATH_ServiceStart;
  static const utils::GUID PATH_ServiceStop;
  static const utils::GUID PATH_ServiceCreate;
  static const utils::GUID PATH_ServiceDestroy;
  static const utils::GUID PATH_GetServiceStatus;
  static const utils::GUID PATH_GetServiceStatusReply;
  static const utils::GUID PATH_ServiceLoaded;
  static const utils::GUID PATH_ServiceStarted;
  static const utils::GUID PATH_ServiceStopped;

  typedef std::map<utils::GUID, IService *> ServiceMap;

private:
  ServiceMap _services;
  std::recursive_mutex _serviceLock;
  std::recursive_mutex _actionsLock;

  ServiceContext *_context;

  std::string _managerID;

  enum ActionType { AStartService, AStopService, ARemoveService };
  struct ServiceAction {
    ActionType type;
    std::string service;
  };

  std::vector<ServiceAction> _actions;
  void _AddServiceAction(ActionType t, const std::string &service) ;

public:
  ServiceManager();
  virtual ~ServiceManager();

  virtual ServiceContext *GetContext() { return _context; }

  const std::string &GetManagerID() { return _managerID; }

  /**
   * @brief Load service using a type and parameters
   *
   * @param type type of the service to create
   * @param params parameters to initialize the service
   * @return IService*
   */
  virtual IService *LoadService(const std::string &type, JSONObject &params);
  /**
   * @brief Load services from a property_tree, each node contains the service
   * properties
   *
   * @param tree: json property tree
   */
  virtual void LoadServicesJSON(
      const JSONObject &tree); // Load services from a property tree

  /**
   * @brief Load services from a json file
   *
   * @param path: path to the json file relative from the executable path, or
   * in absolute format
   */
  virtual void LoadServices(const std::string &path); // Load services from path

  /**
   * @brief Called once per frame to update the services, usually called on
   * the main thread to allow the services to execute actions on the main
   * thread (e.g. rendering thread)
   *
   * @param dt: delta time, or time per frame. Allows the services to execute
   * time-based functions
   */
  virtual void Update(float dt);

  /**
   * @brief Get the number of services loaded
   *
   * @return int
   */
  virtual int GetActiveServicesCount();

  /**
   * @brief Get the Service by Index, for iterative usages
   *
   * @param index: index of the service
   *
   * @return IService*
   */
  virtual IService *GetServiceByIndex(int index);

  /**
   * @brief Returns a service based on its alias name
   *
   * @param name: alias of the service
   * @return IService*
   */
  virtual IService *GetService(const utils::GUID &name);

  virtual void AddService(IService *service);

  /**
   * @brief Starts a service based on its alias name
   *
   * @param name: alias of the service
   * @return true: Service started successfully
   * @return false: Service not found or failed to start
   */
  virtual bool StartService(const utils::GUID &name);
  virtual bool StartService(IService *service);
  /**
   * @brief Stops a service based on its alias name
   *
   * @param name: alias of the service
   * @return true: Service stopped successfully
   * @return false: Service not found or failed to stop
   */
  virtual bool StopService(const utils::GUID &name);
  virtual bool StopService(IService *service);
  /**
   * @brief Removes a service based on its alias name
   *
   * @param name: alias of the service
   * @return true: Service removed successfully
   * @return false: Service not found
   */
  virtual bool RemoveService(const utils::GUID &name);

  /**
   * @brief Stop all services
   *
   */
  virtual void StopAll();
  /**
   * @brief Start all services unless default only parameter is set to True.
   * If true, then only services with defaultRunning property will start
   *
   * @param defaultOnly
   */
  virtual void StartAll(bool defaultOnly);
  /**
   * @brief Stop and remove all services
   *
   */
  virtual void Shutdown();

public:
  /**
   * @brief Call back function for service message bus
   *
   * @param serviceName
   * @param path
   * @param data
   * @param address
   * @param forwarded
   */
  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data, uint address,
                                    bool forwarded);
};

} // namespace services
} // namespace avatarflow

#endif //__SERVICEMANAGER__
