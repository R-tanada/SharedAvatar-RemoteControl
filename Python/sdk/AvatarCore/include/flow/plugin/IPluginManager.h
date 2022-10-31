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

#ifndef __IPLUGINMANAGER__
#define __IPLUGINMANAGER__

#include <vector>

#include "IPlugin.h"
#include "utils/ISingleton.h"

namespace avatarflow {

class AvaFlow_API IPluginManager : public utils::ISingleton<IPluginManager> {
   private:
    /* data */
   public:
    IPluginManager(/* args */) {}
    virtual ~IPluginManager() {}

    virtual IPlugin* AddPlugin(const std::string& path) = 0;
    virtual bool AddPlugin(IPlugin* plugin) = 0;
    virtual std::vector<IPlugin*> LoadPlugins(const std::string& path) = 0;

    virtual bool RemovePlugin(const std::string& name) = 0;
    virtual IPlugin* GetPlugin(const std::string& name) = 0;

    virtual void ClearPlugins() = 0;

    virtual std::vector<IPlugin*> GetPluginList() = 0;
};

}  // namespace avatarflow

#endif  //__IPLUGINMANAGER__