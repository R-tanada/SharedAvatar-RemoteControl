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

#ifndef __ISERVICE__
#define __ISERVICE__

#include <string>

#include "IServiceSubscriber.h"
#include "flow/avatarflow_config.h"
#include "utils/IObject.h"
#include "utils/JSONUtils.h"
#include "utils/RTTI.h"
#include "utils/TypedProperty.h"
#include "utils/base.h"

// conflicting with Windows definition...
#undef StartService

namespace avatarflow {
namespace services {
class ServiceContext;

class AvaFlow_API IService : public utils::IObject, public IServiceSubscriber {
  DECLARE_RTTI

  DECLARE_PROPERTY_TYPE(Alias, std::string, )
  DECLARE_PROPERTY_TYPE(DefaultRunning, bool, )

  static utils::GUID PATH_SetServiceSettings;

private:
  static int _aliasIncremental;

  /* data */
  utils::GUID _id;
  std::string _alias;

  bool _defaultRunning;

  typedef std::list<std::string> DependencyList;
  typedef std::map<std::string, DependencyList> DependencyMap;

  DependencyMap _dependencies;

protected:
  ServiceContext *_context;

  virtual void _setupProps();

public:
  IService(const std::string &name);

  virtual ~IService();
  DECLARE_SETGET_PROP(Alias, std::string)
  DECLARE_SETGET_PROP(DefaultRunning, bool)

  ServiceContext *GetContext() { return _context; }
  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();

  virtual void AddDependencyService(const std::string &event,
                                    const std::string &name);
  virtual void RemoveDependencyService(const std::string &event,
                                       const std::string &name);

  /**
   * @brief Update service, usually called by ServiceManager
   *
   * @param dt
   */
  virtual void Update(float dt) {}
  virtual bool IsRunning() = 0;

  virtual void LoadSettings(const JSONObject &settings);
  virtual const utils::GUID &GetServiceGUID() const { return _id; }

  virtual std::string GetPropertyValue(const std::string &property);

  virtual void OnSettingsUpdated(const JSONObject &settings) {}

  static void PublishSettings(const utils::GUID &source,
                              const utils::GUID &targetVideo,
                              const JSONObject &settings);
  /////////

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
};

class IServiceFactory {
  utils::GUID _id;

public:
  IServiceFactory(const utils::GUID &id) : _id(id) {}
  virtual ~IServiceFactory() {}
  virtual utils::GUID GetID() { return _id; }
  virtual IService *CreateService() = 0;
};

#define MAKE_SERVICE_FACTORY(type, factoryID)                                  \
  class type##Factory : public avatarflow::services::IServiceFactory {         \
  public:                                                                      \
    type##Factory() : IServiceFactory(utils::GUID(factoryID)) {}               \
    virtual avatarflow::services::IService *CreateService() {                  \
      return new type();                                                       \
    }                                                                          \
  };

MakeSharedPtr(IService)

} // namespace services
} // namespace avatarflow

#endif
