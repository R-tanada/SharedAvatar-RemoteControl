//
// Created by Chamika Deshan on 5/27/21.
//

#ifndef GSPDECORATORFAKE_HPP
#define GSPDECORATORFAKE_HPP

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"

namespace avatarflow {
    namespace media {

class GSPDecoratorFake : public GSPipelineDecorator {
    friend class GSPDecoratorBuilder;

public:
    virtual ~GSPDecoratorFake();

    GstElement* decorate();

protected:
    GSPDecoratorFake(IGSPipeline* inner);
};

    }  // namespace media
}  // namespace avatarflow


#endif //GSPDECORATORFAKE_H
