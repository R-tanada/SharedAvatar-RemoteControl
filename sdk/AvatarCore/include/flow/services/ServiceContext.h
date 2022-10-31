

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

#ifndef __SERVICECONTEXT__
#define __SERVICECONTEXT__

namespace avatarflow {
namespace services {
class IServiceManager;
class ServiceCoordinator;

class ServiceContext {
public:
  IServiceManager *manager;
  ServiceCoordinator *coordinator;

public:
  ServiceContext() {
    manager = 0;
    coordinator = 0;
  }
  ~ServiceContext() {}
};

} // namespace services

} // namespace avatarflow

#endif //__SERVICECONTEXT__