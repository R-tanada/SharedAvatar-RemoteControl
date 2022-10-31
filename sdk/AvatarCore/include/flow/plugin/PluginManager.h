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

#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include "IPluginManager.h"

namespace avatarflow {

class AvaFlow_API PluginManager : public IPluginManager {
   private:
    std::vector<IPlugin*> _plugins;

   public:
    PluginManager(/* args */);
    virtual ~PluginManager();

    virtual IPlugin* AddPlugin(const std::string& path);
    virtual bool AddPlugin(IPlugin* plugin);
    virtual std::vector<IPlugin*> LoadPlugins(const std::string& path);

    virtual void ClearPlugins();

    virtual bool RemovePlugin(const std::string& name);
    virtual IPlugin* GetPlugin(const std::string& name);

    virtual std::vector<IPlugin*> GetPluginList();
};

}  // namespace avatarflow

#endif  //__PLUGINMANAGER__