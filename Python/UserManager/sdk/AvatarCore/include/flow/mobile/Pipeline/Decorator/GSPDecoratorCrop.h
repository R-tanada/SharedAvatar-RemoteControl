//
//  GSPDecoratorCrop.h
//  avatarcore_ios
//
//  Created by Chamika Deshan on 5/25/21.
//

#ifndef GSPIPELINE_DECORATOR_CROP_HPP
#define GSPIPELINE_DECORATOR_CROP_HPP

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"

namespace avatarflow {
namespace media {

class GSPDecoratorCrop : public GSPipelineDecorator {
    friend class GSPDecoratorBuilder;

   public:
    virtual ~GSPDecoratorCrop();

    GstElement* decorate();

   protected:
    GSPDecoratorCrop(IGSPipeline* inner, int left, int right, int top, int bottom);

    int v_left;
    int v_right;
    int v_top;
    int v_bottom;
};
}  // namespace media
}  // namespace avatarflow

#endif /* GSPDecoratorAspectRatio_hpp */
