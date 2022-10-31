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
 ************************************************************************/

#ifndef __CoreAPISingleton__
#define __CoreAPISingleton__

#include "coreAPI/avatarcore_config.h"
#include <utils/ISingleton.h>
#include <utils/JSONUtils.h>
#include <utils/ListenerContainer.h>

#include "../AppLogDevice.h"
#include "IResourceHandlerListener.h"

class CoreAPISingletonImpl;

struct DeviceInfo {
public:
  std::string name;
  std::string code;
};

namespace avatarflow {
namespace services {
class IService;
}
} // namespace avatarflow

/**
 * @brief This class is mainly used for wrapper libraries (e.g. nodeJS,
 * Unity3D), it provides common API needed to work with the core. Its a
 * singleton so only one instance can be created per process.
 *
 */
class AvaCore_API CoreAPISingleton
    : public utils::ISingleton<CoreAPISingleton>,
      public utils::ListenerContainer<ICoreAPIWrapperListener *> {
  CoreAPISingletonImpl *_impl;

  // static Napi::Env _env;

  static int _refCount;

  CoreAPISingleton();

public:
  static bool _destroyed;

  static void StartCloud();

  static void cleanUp();
  static void KillProcess();
  static void CloseCloud();
  static void SetServiceMode(bool on);

  DECLARE_FIRE_METHOD(OnAPIClosing, (), ());

public:
  ~CoreAPISingleton();
  /**
   * @brief Check if the API has been inited by a "caller" function. If not
   * inited then it will print out an error with the caller name to stdout
   *
   * @param caller
   * @return true
   * @return false
   */
  bool CheckInited(const std::string &caller);

  /**
   * @brief Initialize core API
   *
   * @param master is it a master process
   * @param path path of application data folder
   * @param port port to be assigned to
   * @return true
   * @return false
   */
  bool InitAPI(bool master, const std::string &path, int port,
               const std::string &logPath = ".",
               utils::AppLogDevice::LoggerFunction logCB = 0,
               const std::string &appName = "");

  /**
   * @brief Destroy and clean up core API
   *
   */
  void DestroyAPI();

  /**
   * @brief load services from a path into the same process (internal service
   * manager). This will not create a new service process
   *
   * @param path relative path to the services to be loaded
   */
  void LoadServices(const std::string &path);

  /**
   * @brief load services from property tree
   *
   * @param tree JSONObject of the service json
   */
  void LoadServicesJSON(const JSONObject &tree);

  /**
   * @brief Start up all default services (defaultRunning=true)
   *
   */
  void StartDefaultServices();

  /**
   * @brief Launches or references a new subprocess with the service name to be
   * loaded. This WILL create a new subprocess
   *
   * @param service
   */
  void RefService(const std::string &service);

  /**
   * @brief Dereference a service which may result in closing it if no other
   * part of the code is referencing it
   *
   * @param service
   */
  void UnrefService(const std::string &service);

  /**
   * @brief Destroy all launched services by this process
   *
   */
  void TerminateAllServices();

  /**
   * @brief adds a service locally (same process)
   *
   * @param service
   * @return true
   * @return false
   */
  bool AddService(avatarflow::services::IService *service);

  /**
   * @brief get a service
   *
   * @param service Name (Alias) of the service
   */
  avatarflow::services::IService *GetService(const std::string &service);

  /**
   * @brief removes a service locally (same process)
   *
   * @param service
   * @return true
   * @return false
   */
  bool RemoveService(avatarflow::services::IService *service);

  /**
   * @brief remove all loaded services in the local process (not external sub
   * processes)
   *
   */
  void RemoveAllServices();

  /**
   * @brief Start a service (calling Start) in a loaded service either in the
   * same process or any other avatar service
   *
   * @param service Name (Alias) of the service
   */
  void RunService(const std::string &service);
  /**
   * @brief Stop a service (calling Start) in a loaded service either in the
   * same process or any other avatar service
   *
   * @param service Name (Alias) of the service
   */
  void StopService(const std::string &service);

  /**
   * @brief Publish a message via service coordinator to subscribers matching
   * "target" regex, or "" to be a broadcast
   *
   * @param target Name of the subscriber to receive the message, it can be a
   * regex e.g. Capture.* (maching any service starting with Capture), or "" to
   * be a broadcast
   * @param path Message path
   * @param data Data passed along the message
   */
  void PublishMessage(const std::string &target, const std::string &path,
                      const std::string &data);

  /**
   * @brief Subscribe to a message (path) on the service coordinator message bus
   *
   * @param name Name of the subscriber
   * @param path Message to be subscribing into
   * @param sub subscriber object
   * @return true
   * @return false
   */
  bool SubscribeToMessage(const std::string &name, const std::string &path,
                          ICoreAPIServiceSubscriber *sub);

  /**
   * @brief Unsubscribe to a message (path) on the service coordinator message
   * bus
   *
   * @param name Name of the subscriber
   * @param path Message to unsubscribe of
   * @return true
   * @return false
   */
  bool UnsubscribeToMessage(const std::string &name, const std::string &path);

  /**
   * @brief Unsubscribe from the service coordinator messages
   * bus
   *
   * @param name Name of the subscriber
   * @return true
   * @return false
   */
  bool Unsubscribe(const std::string &name);

  /**
   * @brief Update loop tick for services, should be called every frame from
   * main thread
   *
   */
  void UpdateLoop();

  /**
   * @brief not used
   *
   * @param ip
   * @param message
   */
  void InvokeRemoteMessage(const std::string &ip, const std::string &message);

  /**
   * @brief Increase ref count of the CoreAPI singelton, if its called for first
   * time then it will create an instance of it
   *
   * @return int Ref count after increased, if its 1 then it has just been
   * created
   */
  static int Ref();

  /**
   * @brief Decrease ref count of the CoreAPI singelton, if it reached 0 then it
   * will destroy it
   *
   * @return int Ref count after decreased, if its 0 then it has just been
   * destroyed
   */
  static int Unref();

  /**
   * @brief Current ref count of this manager
   *
   * @return int
   */
  static int RefCount();

  // Application Configurations Utilities
  /**
   * @brief Set the App Configuration File Path to read and write into, it
   * represents a json object
   *
   * @param path path of the configuration file
   * @param relativeAppPath will it be loaded from relative path or absolute
   * path
   */
  void SetAppConfigurationFilePath(const std::string &path,
                                   bool relativeAppPath);

  /**
   * @brief Set a key:value pair into the configuration file
   *
   * @param name
   * @param value
   */
  void SetAppConfiguation(const std::string &name, const std::string &value);

  /**
   * @brief Retrieve the value of a provided key, if the key is not found then
   * it will return defaultValue
   *
   * @param name name of the key to retrieve its value
   * @param defaultValue returned if "name" was not found
   * @return std::string
   */
  std::string GetAppConfiguation(const std::string &name,
                                 const std::string &defaultValue = "");

  /**
   * @brief Get the Device ID of the core (only valid in core Hardware)
   *
   * @return const std::string&
   */
  const std::string &GetDeviceID();

  /**
   * @brief Get IP address of a network interface
   *
   * @param iface network interface to get assigned IP of
   * @return std::string
   */
  std::string GetIPAddress(const std::string &iface);

  /**
   * @brief Get MAC address of a network interface
   *
   * @param iface network interface to get assigned MAC of
   * @return std::string
   */
  std::string GetMACAddress(const std::string &iface);

  /**
   * @brief Returns the version of an SDK \n
   * "core": AvatarCore SDK version \n
   * "stream": StreamSDK version
   *
   * @param sdk Name of the SDK to get its version
   * @return std::string
   */
  std::string GetSDKVersion(const std::string &sdk);

  /**
   * @brief Add a log message to the logmanager, this would allow high level
   * application to log directly via the native manager
   *
   * @param category
   * @param msg
   * @param level level of the log (0~7 - Refer to utils::ELogLevel)
   * @param verbose verbosity of the message, higher values means more verbose
   * information
   */
  void LogMessage(const std::string &category, const std::string &msg,
                  int level, int verbose);

  ////////// Device management

  /**
   * @brief Refresh device manager
   *
   */
  void RefreshDevices();
  /**
   * @brief Get the available camera devices
   *
   * @param src
   * @return int
   */
  int GetCameraList(std::vector<DeviceInfo> &src);
  /**
   * @brief Get the available audio output interfaces
   *
   * @param src
   * @return int
   */
  int GetSpeakerList(std::vector<DeviceInfo> &src);
  /**
   * @brief Get the available audio input interfaces
   *
   * @param src
   * @return int
   */
  int GetMicList(std::vector<DeviceInfo> &src);

public:
};
#define gCoreAPISingleton CoreAPISingleton::getInstance()
#endif //__CoreAPISingleton__
