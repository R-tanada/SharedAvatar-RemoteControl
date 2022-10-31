#ifndef DataWrap_H
#define DataWrap_H

#include <coreAPI/Wrappers/IResourceHandlerListener.h>
#include <utils/ListenerContainer.h>
#include <coreAPI/Wrappers/CoreDataResource.h>
#include <flow/media/Resources/ResourceManager.h>
#include <mobile/TemplateAsyncListerner.h>

class CoreResourceHandler;

class DataWrap : public CoreDataResource {
   public:
    
    DataWrap(IResourceHandlerListener *resourceListner);

    ~DataWrap();

    bool startStreaming(const std::string &resourceName,
                        const std::string &channel, int fetchRate);
    bool stopStreaming();

    bool sendData(const std::string &resource, const std::string &channelName,
                  const std::string &data);

    void Destroy();

    // CoreDataResource
    void OnDataFrame(CoreResourceHandler *self, const std::string &resourceName,
                     const std::string &channelName,
                     avatarflow::media::DataFrame *frame);
    
    void triggerDataReceived(std::string &tag);
    
private:
    IResourceHandlerListener *mResourceListner;
    
    avatarmobile::TemplateAsyncListerner<DataWrap>* asyncListner;
};

#endif
