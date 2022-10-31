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

#ifndef __CONFIGURATIONMANAGER__
#define __CONFIGURATIONMANAGER__

#include "Capabilities.h"
#include "GUID.h"

namespace utils {

class AvaUtils_API ConfigurationManager {
   private:
   public:
    // loggers

    static Capabilities LogConfiguration;
    static std::string LOG_ROOT;

    static std::string LOG_VERBOSE_LEVEL;
    static std::string LOG_PATH;
    static std::string LOG_TIMESTAMP;

   public:
    static void LoadSettings(const std::string& basePath = ".");
};

#define gLogConfig utils::ConfigurationManager::LogConfiguration

#define SERVICE_CAN_LOG(Service) \
    (gLogConfig.GetCapability(Service->GetServiceGUID().GetString(), true))

}  // namespace utils

#endif  //__CONFIGURATIONMANAGER__