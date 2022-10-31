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

#ifndef __IRESOURCE__
#define __IRESOURCE__

#include "avataros_config.h"
#include <string>
#include <time.h>
#include <utils/GUID.h>
#include <utils/ListenerContainer.h>
#include <utils/atypes.h>

namespace avatarOS {

class AvaOS_API IResourceManager;
class AvaOS_API IResource;
class AvaOS_API IAsyncLoadRequest;

typedef utils::ulong ResourceHandle;
class IResourceListner {
public:
  virtual ~IResourceListner() {}
  virtual void OnResourceLoadComplete(IResource *) = 0;
  virtual void OnResourceUnloadComplete(IResource *) = 0;
};

class AvaOS_API IResource
    : public utils::ListenerContainer<IResourceListner *> {

protected:
  DECLARE_FIRE_METHOD(OnResourceLoadComplete, (IResource * r), (r))
  DECLARE_FIRE_METHOD(OnResourceUnloadComplete, (IResource * r), (r))
public:
  enum EPriorityLevel { EPL_LowLevel = 0, EPL_MidLevel, EPL_HighLevel };
  enum ELoadingState {
    LoadState_Unloaded,
    LoadState_Loading,
    LoadState_Loaded,
    LoadState_Unloading
  };

protected:
  IResourceManager *m_manager;
  IAsyncLoadRequest *m_loadRequest;

  EPriorityLevel m_priorityLevel;
  time_t m_lastAccess;

  utils::GUID m_handle;
  std::string m_name;
  std::string m_group;

  ELoadingState m_loadState;

  utils::uint m_size;

  // this number determines number of objects who need this resource,when it
  // reaches 0,it'll call unload method to free up memory if it raised to 1
  // again,it'll reload the resource
  utils::uint m_usedCounter;

  virtual utils::uint calcSizeInternal() = 0;

  virtual void preLoadInternal(bool async);
  virtual void loadInternal();
  virtual void postLoadInternal();

  virtual void preUnloadInternal();
  virtual void unloadInternal();
  virtual void postUnloadInternal();

  void fillProperties();

  void notifyResourceListners(bool load);

  void setResourceManager(IResourceManager *manager);

  friend class AvaOS_API IResourceManager;

  void _onLoaded(); // must be called after it is loaded
public:
  IResource();

  virtual ~IResource();

  virtual void _onResourceUsed();

  virtual void useResource();
  virtual void unuseResource();

  virtual bool load(bool async = true);

  virtual void unload();
  virtual bool reload();

  virtual utils::uint recalcSize() {
    m_size = calcSizeInternal();
    return m_size;
  }

  ELoadingState getResourceState() const;

  virtual utils::uint getResourceSize() const { return m_size; }

  inline void setResourcePriority(EPriorityLevel level) {
    m_priorityLevel = level;
  }
  inline EPriorityLevel getResourcePriority() const { return m_priorityLevel; }

  inline void setLastResourceAccess(time_t lastAccess) {
    m_lastAccess = lastAccess;
  }
  inline time_t getLastResourceAccess() const { return m_lastAccess; }

  void setResourceHandle(const utils::GUID &handler);
  void setResourceName(const std::string &name);
  void setResourceGroup(const std::string &group);

  inline const utils::GUID &getResourceHandle() const { return m_handle; }

  const std::string &getResourceName() const { return m_name; }

  inline const std::string &getResourceGroup() const { return m_group; }

  virtual bool operator<(const IResource &o) const;
};

} // namespace avatarOS

#endif
