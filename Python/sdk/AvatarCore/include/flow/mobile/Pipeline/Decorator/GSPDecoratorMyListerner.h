#ifndef GSPIPELINE_DECORATOR_MY_LISTERNER_HPP
#define GSPIPELINE_DECORATOR_MY_LISTERNER_HPP

#include <gst/app/gstappsink.h>

#include <iostream>

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"
#include "flow/media/GStreamer/GstMyListener.h"

namespace avatarflow {
namespace media {

class GSPDecoratorMyListerner : public GSPipelineDecorator{
    friend class GSPDecoratorBuilder;

   public:

    virtual ~GSPDecoratorMyListerner();

    GstElement* decorate();
    
   protected:
    GSPDecoratorMyListerner(IGSPipeline* inner, media::IMyListenerCallback *callback, const gchar *eleName);

   protected:

    // Avatar Core
    media::IMyListenerCallback *mListernerCallback;
    const gchar *elementName;
};
}  // namespace media
}  // namespace avatarflow
#endif
