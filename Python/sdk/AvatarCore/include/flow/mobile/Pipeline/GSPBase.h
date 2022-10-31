//
//  GSPDecoratorCrop.h
//  avatarcore_ios
//
//  Created by Chamika Deshan on 5/25/21.
//

#ifndef GSPBase_hpp
#define GSPBase_hpp

#include "./Builder/GSPDecoratorBuilder.h"
//#include "GSPipelineDecorator.h"
#include "IGSPipeline.h"
#include <flow/media/GStreamer/GstPipeline.h>

namespace avatarflow {
namespace media {

class GSPBase : public IGSPipeline {
    friend class GSPDecoratorBuilder;

   public:
    virtual ~GSPBase();

    virtual GstElement* decorate();

   protected:
    GSPBase(media::GstPipeline* inner);
    media::GstPipeline* mGsPipeline;
};
}  // namespace media
}  // namespace avatarflow

#endif /* GSPDecoratorBase_hpp */
