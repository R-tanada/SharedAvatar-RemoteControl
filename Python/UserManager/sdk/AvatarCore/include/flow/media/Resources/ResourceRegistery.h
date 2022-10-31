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

#ifndef __RESOURCEREGISTERY__
#define __RESOURCEREGISTERY__

#include <utils/ISingleton.h>

#include "CoreResource.h"

namespace avatarflow {
namespace media {

class ResourceRegistery : public utils::ISingleton<ResourceRegistery*> {
   private:
    /* data */
   public:
    ResourceRegistery(/* args */);
    virtual ~ResourceRegistery();

    void ReferenceResource(const std::string& resourceName);
    void UnreferenceResource(const std::string& resourceName);
};

}  // namespace media

}  // namespace avatarflow

#endif  //__RESOURCEREGISTERY__