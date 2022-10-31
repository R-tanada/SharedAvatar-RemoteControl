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

#ifndef __IPLUGIN__
#define __IPLUGIN__

#include <utils/OSType.h>

#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {

class AvaFlow_API IPlugin {
   private:
    bool _installed;

   public:
    IPlugin();
    virtual ~IPlugin();

    virtual std::string GetName() = 0;

    virtual bool IsInstalled();
    virtual void Install();
    virtual void Uninstall();
};  // namespace avatarflow

typedef IPlugin* (*PluginCreateFunc)();

#ifdef __WINDOWS__
#define IMPLEMET_PLUGIN_FUNCTION(PluginName)                               \
    extern "C" __declspec(dllexport) avatarflow::IPlugin* CreatePlugin() { \
        return new PluginName();                                           \
    }
#else
#define IMPLEMET_PLUGIN_FUNCTION(PluginName)                               \
    extern "C" __attribute__((visibility("default"))) avatarflow::IPlugin* \
    CreatePlugin() {                                                       \
        return new PluginName();                                           \
    }

#endif

}  // namespace avatarflow

#endif  //__IPLUGIN__