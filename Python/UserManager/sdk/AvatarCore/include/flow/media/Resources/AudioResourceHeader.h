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

#ifndef __AudioResourceHeader__
#define __AudioResourceHeader__

#include "IResourceHeader.h"
#include "flow/media/Resources/AudioFrame.h"

namespace avatarflow {
namespace media {

class AudioResourceHeader : public IResourceHeader {
public:
  AudioResourceHeader()
      : IResourceHeader(CoreResourceType::Resource_Audio), currentTop(0) {}
  AudioFrame frame;
  utils::uint currentTop;
};

} // namespace media
} // namespace avatarflow

#endif //__AudioResourceHeader__