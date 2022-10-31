#ifndef GSPIPELINE_DECORATOR_GLSINK_CLIENT_HPP
#define GSPIPELINE_DECORATOR_GLSINK_CLIENT_HPP

#include <gst/gst.h>

#include <iostream>

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"

namespace avatarflow {
namespace media {

class GSPDecoratorGLSinkClient : public GSPipelineDecorator {
    friend class GSPDecoratorBuilder;

   public:
    virtual ~GSPDecoratorGLSinkClient();

    GstElement* decorate();

   protected:
    GSPDecoratorGLSinkClient(IGSPipeline* inner, GCallback pReshapeCallback, GCallback pDrawCallback);

    GCallback reshapeCallback;
    GCallback drawCallback;
};
}  // namespace media
}  // namespace avatarflow
#endif
