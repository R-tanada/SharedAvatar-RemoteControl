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

#ifndef __DYNAMICLIBRARYMANAGER__
#define __DYNAMICLIBRARYMANAGER__

#include <vector>

#include "IPlugin.h"
#include "utils/ISingleton.h"

namespace avatarflow {

class DynamicLibrary;

class AvaFlow_API DynamicLibraryManager
    : public utils::ISingleton<DynamicLibraryManager> {
private:
  std::vector<DynamicLibrary *> _libs;

public:
  DynamicLibraryManager(/* args */);
  ~DynamicLibraryManager();

  DynamicLibrary *GetLibrary(const std::string &path);
};

} // namespace avatarflow

#endif //__DYNAMICLIBRARYMANAGER__