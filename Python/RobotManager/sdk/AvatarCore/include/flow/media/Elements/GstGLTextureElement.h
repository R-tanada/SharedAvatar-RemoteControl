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

#ifndef __GSTGLTEXTUREELEMENT__
#define __GSTGLTEXTUREELEMENT__

#include "flow/avatarflow_config.h"
#include <string>

namespace avatarflow {
namespace media {

class IVideoSourceDevice;
class GstGLTextureElementImpl;
class GstPipeline;

class AvaFlow_API GstGLTextureElement {
private:
  GstGLTextureElementImpl *_impl;

public:
  GstGLTextureElement(/* args */);
  ~GstGLTextureElement();

  void SetTextureSource(IVideoSourceDevice *device);

  unsigned int GetTextureID();

  std::string BuildPipeline(const std::string &name);
  void LinkWithPipeline(GstPipeline *pipeline);
};

} // namespace media
} // namespace avatarflow

#endif //__GSTGLTEXTUREELEMENT__