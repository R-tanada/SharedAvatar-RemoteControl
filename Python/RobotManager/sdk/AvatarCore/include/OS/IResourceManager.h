
#ifndef ___IResourceManager___
#define ___IResourceManager___

//#include <hash_map>
#include "IResource.h"
#include "IStream.h"
#include <mutex>

namespace avatarOS {

class AvaOS_API IResourceManager {

public:
  typedef std::map<ResourceHandle, IResource *> ResourceHandleMap;
  typedef std::map<std::string, IResource *> ResourceMap;

protected:
  ResourceHandle m_lastHandle;

  ResourceHandleMap m_resourceHandleMap;
  ResourceMap m_resourceMap;

  utils::ulong m_allowedMemory;
  utils::ulong m_usedMemory;

  std::mutex m_resMutex;

  std::string m_resourceType;

  ResourceHandle GetNextHandle();

protected:
  virtual IResource *CreateInternal(const std::string &name) = 0;
  virtual void RemoveInternal(IResource *resource);
  virtual bool AddInternal(IResource *resource, const std::string &name);

  virtual IResource *LoadResourceFromFile(IStream *file) = 0;

  virtual IResource *Create(const std::string &name);

  virtual std::string GetDefaultGroup() = 0;

  void _removeResource(IResource *resource);

public:
  IResourceManager(const std::string &resourceType);
  virtual ~IResourceManager();

  virtual IResource *GetOrCreate(const std::string &name);
  virtual IResource *CreateFromFile(IStream *file);

  virtual void SetAllowedMemory(utils::ulong size);
  virtual utils::ulong GetAllowedMemory();
  virtual void CheckMemory();
  virtual utils::ulong GetUsedMemory(bool recalc = false);
  /**/
  virtual void Unload(const std::string &name);
  virtual void Unload(IResource *resource);
  virtual void Unload(ResourceHandle resource);

  virtual void UnloadAll(bool unusedOnly = true);

  virtual void Reload(const std::string &name);
  virtual void Reload(IResource *resource);
  virtual void Reload(ResourceHandle resource);
  virtual void ReloadAll();

  virtual void AddResource(IResource *resource, const std::string &name);

  virtual void Remove(const std::string &name);
  virtual void Remove(IResource *resource);
  virtual void Remove(ResourceHandle resource);
  virtual void RemoveAll();

  virtual void WriteResourceToDist(const std::string &resName,
                                   const std::string &fileName){};
  virtual void WriteResourceToDist(IResource *resource,
                                   const std::string &fileName){};
  virtual void WriteResourceToDist(ResourceHandle resource,
                                   const std::string &fileName){};

  virtual IResource *GetResource(const std::string &name);
  virtual IResource *GetResourceByHandle(ResourceHandle resource);

  virtual bool IsResourceExists(const std::string &name);
  virtual bool IsResourceExists(ResourceHandle resource);

  virtual void ResourceLoaded(IResource *res);
  virtual void ResourceUnloaded(IResource *res);

  virtual void _requestLoad(IResource *r);
  virtual void _requestUnload(IResource *r);

  virtual void ListResources(std::vector<std::string> &outList);
  virtual void ListResources(std::vector<IResource *> &outList);

  const std::string &GetResourceType();
};

} // namespace avatarOS

#endif
