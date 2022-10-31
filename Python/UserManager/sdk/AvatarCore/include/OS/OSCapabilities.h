
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

#ifndef __OSCAPABILITIES__
#define __OSCAPABILITIES__

#include <utils/Capabilities.h>

#include "avataros_config.h"

namespace avatarOS {

class AvaOS_API OSCapabilities : public utils::Capabilities {
   private:
   public:
    OSCapabilities();
    ~OSCapabilities();

   public:
    // some default capabilities names

    DECLARE_CAPABILITY_NAME(HardwareEncoding);
    DECLARE_CAPABILITY_NAME(HardwareDecoding);
    DECLARE_CAPABILITY_NAME(WiFi);
    // static const utils::GUID CAP_;
};

}  // namespace avatarOS

#endif  //__OSCAPABILITIES__
