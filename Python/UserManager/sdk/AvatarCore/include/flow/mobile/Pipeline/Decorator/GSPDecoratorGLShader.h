#ifndef GSPIPELINE_DECORATOR_GLSHADER_HPP
#define GSPIPELINE_DECORATOR_GLSHADER_HPP

#include <iostream>

#include "../Builder/GSPDecoratorBuilder.h"
#include "GSPipelineDecorator.h"

namespace avatarflow {
namespace media {

class GSPDecoratorGLShader : public GSPipelineDecorator {
    friend class GSPDecoratorBuilder;

   public:
    virtual ~GSPDecoratorGLShader();

    GstElement* decorate();

   protected:
    GSPDecoratorGLShader(IGSPipeline* inner, const gchar *eleName, const char* frag, const char* vert);
    GSPDecoratorGLShader(IGSPipeline* inner, const gchar *eleName, const char* frag, const char* vert, const FloatUnifromMap &map);

    const gchar *mElementName;
    const char* mFrag;
    const char* mVert;
    FloatUnifromMap mUniformMap;
};
}  // namespace media
}  // namespace avatarflow
#endif
