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

#ifndef __ServiceFactory__
#define __ServiceFactory__

#include <map>

#include "IService.h"
#include "flow/avatarflow_config.h"
#include "utils/GUID.h"
#include "utils/ISingleton.h"

namespace avatarflow {
namespace services {

class ServiceFactoryImpl;
class IServiceManager;
class AvaFlow_API ServiceFactory : public utils::ISingleton<ServiceFactory> {

  ServiceFactoryImpl *_impl;

public:
  typedef std::map<utils::uint, IServiceFactory *> FactoryMap;

public:
  ServiceFactory(/* args */);
  ~ServiceFactory();

  void RegisterFactory(IServiceFactory *factory);
  void RemoveFactory(utils::GUID id);
  void ClearFactories();
  const FactoryMap &GetFactories() const;

  IServiceFactory *GetFactory(utils::GUID id);

  IServiceManager *CreateServiceManager();
};
#define gServiceFactory avatarflow::services::ServiceFactory::getInstance()

} // namespace services
} // namespace avatarflow

#endif //__ServiceFactory__