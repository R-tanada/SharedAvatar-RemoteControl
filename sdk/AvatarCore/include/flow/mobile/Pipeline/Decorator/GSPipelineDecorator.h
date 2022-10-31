#ifndef GSPIPELINE_DECORATOR_HPP
#define GSPIPELINE_DECORATOR_HPP

#include <gst/gst.h>

#import <flow/media/GStreamer/GstPipeline.h>
#include "../IGSPipeline.h"
// Has Decorator Design Pattern

namespace avatarflow {
namespace media {

class GSPipelineDecorator : public IGSPipeline{
   public:
    virtual ~GSPipelineDecorator();
    
    virtual GstElement*  decorate();

   protected:
    GSPipelineDecorator(IGSPipeline* inner);
    IGSPipeline* mGsPipeline;
    
};
}  // namespace media
}  // namespace avatarflow
#endif
