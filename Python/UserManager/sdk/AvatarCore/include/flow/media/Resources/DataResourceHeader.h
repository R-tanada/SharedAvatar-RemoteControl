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

#ifndef __DataResourceHeader__
#define __DataResourceHeader__

#include "IResourceHeader.h"
#include "flow/media/Resources/DataFrame.h"

namespace avatarflow {
namespace media {

/**
 * @brief an implementation of resource header dedicated for data resources
 *
 */
class DataResourceHeader : public IResourceHeader {
public:
  DataResourceHeader()
      : IResourceHeader(CoreResourceType::Resource_Data), currentTop(0),
        previousTop(0) {}
  DataFrame frame;
  utils::uint currentTop;
  utils::uint previousTop;
};

} // namespace media
} // namespace avatarflow

#endif //__DataResourceHeader__