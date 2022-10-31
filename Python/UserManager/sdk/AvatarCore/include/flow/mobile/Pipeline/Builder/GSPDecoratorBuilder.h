#ifndef GSPIPELINE_DECORATOR_BUILDER_HPP
#define GSPIPELINE_DECORATOR_BUILDER_HPP

#include <gst/gst.h>
#include "flow/mobile/MobileDefs.h"
#include <map>

// Has Builder Design Pattern

namespace avatarflow {
namespace media {
class GstPipeline;
class VideoAppSinkHandler;
class IMyListenerCallback;

typedef std::map<std::string, float> FloatUnifromMap;

class IGSPipeline;

class GSPDecoratorBuilder {
private:
    IGSPipeline* base;
    
public:
    
    GSPDecoratorBuilder(media::GstPipeline* existingPipeline);
    
    ~GSPDecoratorBuilder();
    
    operator IGSPipeline*() const { return std::move(base); }
    
    // Additional Builders
    GSPDecoratorBuilder& appSink(const std::string &sinkName, media::VideoAppSinkHandler* handler);
    GSPDecoratorBuilder& glShader(const char* eleName, const char* fragShader, const char* vertShader);
    GSPDecoratorBuilder& glShader(const char* eleName, const char* fragShader, const char* vertShader, const FloatUnifromMap &map);
    GSPDecoratorBuilder& glSink(guintptr renderViewId);
    GSPDecoratorBuilder& glSinkClient(GCallback pReshapeCallback, GCallback pDrawCallback);
    GSPDecoratorBuilder& crop(int left, int right, int top, int bottom);
    GSPDecoratorBuilder& mylisterner(media::IMyListenerCallback* callback, const gchar *eleName);
    
    // Fake decorator for testing
    GSPDecoratorBuilder& fake();
};
}  // namespace media
}  // namespace avatarflow
#endif
