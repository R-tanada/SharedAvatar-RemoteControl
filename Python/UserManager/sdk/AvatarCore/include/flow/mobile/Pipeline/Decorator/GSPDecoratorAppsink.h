#ifndef GSPIPELINE_DECORATOR_APPSINK_HPP
#define GSPIPELINE_DECORATOR_APPSINK_HPP

#include <gst/app/gstappsink.h>

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"
#include "flow/media/AppSink/VideoAppSinkHandler.h"

namespace avatarflow {
namespace media {

class Async;
class IGSTDataCallback;

class GSPDecoratorAppsink : public GSPipelineDecorator{
    friend class GSPDecoratorBuilder;

   public:

    virtual ~GSPDecoratorAppsink();

    GstElement* decorate();

   protected:
    GSPDecoratorAppsink(IGSPipeline* inner, const std::string &sinkName, media::VideoAppSinkHandler *appSinkHandler);

   protected:

    const std::string mSinkName;
    // Avatar Core
    media::VideoAppSinkHandler *mSinkHandler;
};
}  // namespace media
}  // namespace avatarflow
#endif
