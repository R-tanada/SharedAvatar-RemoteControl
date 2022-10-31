#ifndef GSPIPELINE_DECORATOR_GLSINK_HPP
#define GSPIPELINE_DECORATOR_GLSINK_HPP

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"

namespace avatarflow {
namespace media {

class GSPDecoratorGLSink : public GSPipelineDecorator {
    friend class GSPDecoratorBuilder;

   public:
    virtual ~GSPDecoratorGLSink();

    GstElement* decorate();

   protected:
    GSPDecoratorGLSink(IGSPipeline* inner, guintptr renderViewId);

    guintptr mRenderViewId;
};
}  // namespace media
}  // namespace avatarflow
#endif
