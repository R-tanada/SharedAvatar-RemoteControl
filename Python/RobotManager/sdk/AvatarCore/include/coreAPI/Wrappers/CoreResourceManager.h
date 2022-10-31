#ifndef CoreResourceManager_H
#define CoreResourceManager_H

#include "IResourceHandlerListener.h"
#include "coreAPI/avatarcore_config.h"

class CoreResourceHandler;

class AvaCore_API CoreResourceManager : public IResourceHandlerListener {

public:
  CoreResourceManager();
  virtual ~CoreResourceManager();

  virtual bool Start(int timeout);
  virtual bool Stop();

  int GetResourcesCount();
  const std::string &GetResourceName(int i);
  const std::string &GetChannelName(int i);

  virtual void Destroy();

  bool SubscribeVideo(const std::string &resource);
  bool SubscribeAudio(const std::string &resource);
  bool SubscribeData(const std::string &resource,
                     const std::string &channel);

  bool UnsubscribeVideo(const std::string &resource);
  bool UnsubscribeAudio(const std::string &resource);
  bool UnsubscribeData(const std::string &resource,
                               const std::string &channel);

  virtual bool WriteData(const std::string &resource,
                         const std::string &channel, const std::string &data);
  virtual bool WriteVideo(avatarflow::media::VideoFrame *frame,
                          const std::string &resourceName);
  virtual bool WriteAudio(avatarflow::media::AudioFrame *frame,
                          const std::string &resourceName);

  virtual void OnVideoFrame(CoreResourceHandler *self,
                            const std::string &resource,
                            avatarflow::media::VideoFrame *frame) {}
  virtual void OnAudioFrame(CoreResourceHandler *self,
                            const std::string &resource,
                            avatarflow::media::AudioFrame *frame) {}
  virtual void OnDataFrame(CoreResourceHandler *self,
                           const std::string &resource,
                           const std::string &channel,
                           avatarflow::media::DataFrame *frame) {}

protected:
  CoreResourceHandler *player;
};

#endif
