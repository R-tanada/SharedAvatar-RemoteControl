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

#ifndef __ISERVICEMANAGER__
#define __ISERVICEMANAGER__

#include "IService.h"
#include "utils/GUID.h"

#undef StartService

namespace avatarflow {
namespace services {
class ServiceContext;
class IServiceManager {
private:
  /* data */
public:
  IServiceManager() {}
  virtual ~IServiceManager() {}

  virtual const std::string &GetManagerID() = 0;

  virtual ServiceContext *GetContext() = 0;

  /**
   * @brief Load service using a type and parameters
   *
   * @param type type of the service to create
   * @param params parameters to initialize the service
   * @return IService*
   */
  virtual IService *LoadService(const std::string &type,
                                JSONObject &params) = 0;
  virtual void LoadServicesJSON(
      const JSONObject &tree) = 0; // Load services from a property tree
  virtual void
  LoadServices(const std::string &path) = 0; // Load services from path

  virtual void Update(float dt) = 0;

  virtual int GetActiveServicesCount() = 0;
  virtual IService *GetServiceByIndex(int index) = 0;
  virtual IService *GetService(const utils::GUID &guid) = 0;

  virtual void AddService(IService *service) = 0;
  virtual bool StopService(const utils::GUID &guid) = 0;
  virtual bool StopService(IService *service) = 0;
  virtual bool StartService(const utils::GUID &guid) = 0;
  virtual bool StartService(IService *service) = 0;
  virtual bool RemoveService(const utils::GUID &name) = 0;

  virtual void StopAll() = 0;
  virtual void StartAll(bool defaultOnly) = 0;
  virtual void Shutdown() = 0;
};

} // namespace services
} // namespace avatarflow

#endif //__ISERVICEMANAGER__