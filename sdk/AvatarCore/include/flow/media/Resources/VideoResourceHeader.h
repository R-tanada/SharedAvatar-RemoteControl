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

#ifndef __VIDEOResourceHeader__
#define __VIDEOResourceHeader__

#include "IResourceHeader.h"
#include "flow/media/Resources/VideoFrame.h"

namespace avatarflow {
namespace media {

class VideoResourceHeader : public IResourceHeader {
public:
  VideoResourceHeader() : IResourceHeader(CoreResourceType::Resource_Video) {}
  VideoFrame frame;
};

} // namespace media
} // namespace avatarflow

#endif //__VIDEOResourceHeader__