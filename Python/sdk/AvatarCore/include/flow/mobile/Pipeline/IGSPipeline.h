//
//  IGSPipeline.h
//  avatarcore_ios
//
//  Created by Chamika Deshan on 5/25/21.
//  Root for decoration

#ifndef IGSPipeline_hpp
#define IGSPipeline_hpp

#include <flow/media/GStreamer/GstPipeline.h>

namespace avatarflow {
namespace media {

class IGSPipeline {

   public:
    virtual ~IGSPipeline(){};
    
    virtual GstElement* decorate() = 0;
};
}  // namespace media
}  // namespace avatarflow

#endif /* IGSPipeline_hpp */
