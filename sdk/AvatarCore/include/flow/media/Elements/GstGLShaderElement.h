/************************************************************************
 *                                                                      *
 * Copyright 2020 AvatarIn Inc. Tokyo, Japan. All Rights Reserved       *
 *                                                                      *
 *       _____               __               .___                      *
 *      /  _  \___  ______ _/  |______ _______|   | ____                *
 *     /  /_\  \  \/ |__  \\   __\__  \\_  __ \   |/    \               *
 *    /    |    \   / / __ \|  |  / __ \|  | \/   |   |  \              *
 *    \____|__  /\_/ (____  /__| (____  /__|  |___|___|  /              *
 *            \/          \/          \/               \/               *
 *                                                                      *
 * Developed by: Yamen Saraiji                                          *
 ************************************************************************/

#ifndef __GLSHADERELEMENT__
#define __GLSHADERELEMENT__

#include <string>

#include "flow/avatarflow_config.h"
#include "utils/GUID.h"
#include "utils/matrix4x4.h"
#include "utils/point2D.h"
#include "utils/point3D.h"
#include "utils/quaternion.h"

namespace avatarflow {
namespace media {

class GstPipeline;
class GstGLShaderElementImpl;
class GstGLTextureElement;

class AvaFlow_API GstGLShaderElement {
private:
  GstGLShaderElementImpl *_impl;
  bool _fetchShader();

public:
  GstGLShaderElement(/* args */);
  virtual ~GstGLShaderElement();

  void SetShaders(const std::string &vertex, const std::string &fragment);

  std::string BuildPipeline(const std::string &prefix);

  virtual void LinkWithPipeline(GstPipeline *pipeline);

  void SetUniformValue(const std::string &name, int v);
  void SetUniformValue(const std::string &name, float v);
  void SetUniformValue(const std::string &name, const utils::vector2d &v);
  void SetUniformValue(const std::string &name, const utils::vector3d &v);
  void SetUniformValue(const std::string &name, const utils::quaternion &v);
  void SetUniformValue(const std::string &name, const utils::matrix4x4 &v);
  void SetTexture2DValue(const std::string &name, GstGLTextureElement *texture);

  void UpdateUniforms();
};

} // namespace media
} // namespace avatarflow

#endif //__GLSHADERELEMENT__