#ifndef CoreResourceHandler_H
#define CoreResourceHandler_H

#include "coreAPI/avatarcore_config.h"
#include <utils/ListenerContainer.h>

#include "IResourceHandlerListener.h"

namespace avatarflow {
namespace media {
class ResourceManager;

}
} // namespace avatarflow

class ResourceListener;
class AvaCore_API CoreResourceHandler
    : public utils::ListenerContainer<IResourceHandlerListener *> {

public:
  DECLARE_FIRE_METHOD(OnVideoFrame,
                      (CoreResourceHandler * self, const std::string &resource,
                       avatarflow::media::VideoFrame *frame),
                      (self, resource, frame))

  DECLARE_FIRE_METHOD(OnAudioFrame,
                      (CoreResourceHandler * self, const std::string &resource,
                       avatarflow::media::AudioFrame *frame),
                      (self, resource, frame))

  DECLARE_FIRE_METHOD(OnDataFrame,
                      (CoreResourceHandler * self, const std::string &resource,
                       const std::string &channel,
                       avatarflow::media::DataFrame *frame),
                      (self, resource, channel, frame))

  enum ResourceType { Video, Audio, Data };

public:
  CoreResourceHandler();

  ~CoreResourceHandler();

  void Cleanup();

  bool SubscribeToResource(ResourceType type, const std::string &resourceName,
                           const std::string &channel);

  bool UnsubscribeToResource(const std::string &resourceName,
                             const std::string &channel);

  void UnsubscribeAll();
  bool StartResource(int fetchRateMS);
  bool StopResource();

  avatarflow::media::ResourceManager *GetResourceManager();

  int GetTotalResourcesCount();
  const std::string &GetResourceName(int i = 0);
  const std::string &GetChannelName(int i = 0);

  avatarflow::media::VideoFrame *
  GetLastVideoFrame(const std::string &resourceName = "");
  avatarflow::media::AudioFrame *
  GetLastAudioFrame(const std::string &resourceName = "");
  avatarflow::media::DataFrame *
  GetLastDataFrame(const std::string &resourceName = "",
                   const std::string &channel = "");

protected:
  ResourceListener *_listener;
};

#endif
