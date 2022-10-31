#ifndef CoreDataResource_H
#define CoreDataResource_H

#include "IResourceHandlerListener.h"
#include "coreAPI/avatarcore_config.h"

class CoreResourceHandler;

class AvaCore_API CoreDataResource : public IResourceHandlerListener {

public:
  CoreDataResource();
  virtual ~CoreDataResource();

  virtual bool Start(const std::string &resource, const std::string &channel);
  virtual bool Stop();

  const std::string &GetChannelName();

  virtual void Destroy();

  virtual bool Write(const std::string &resource, const std::string &channel,
                     const std::string &data);

  virtual void OnDataFrame(CoreResourceHandler *self,
                           const std::string &resourceName,
                           const std::string &channelName,
                           avatarflow::media::DataFrame *frame) {}

protected:
  CoreResourceHandler *player;
};

#endif
