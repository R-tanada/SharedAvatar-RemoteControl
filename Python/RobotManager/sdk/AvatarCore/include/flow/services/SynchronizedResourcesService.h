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

#ifndef __SYNCHRONIZEDRESOURCESSERVICE__
#define __SYNCHRONIZEDRESOURCESSERVICE__

#include "IService.h"

namespace avatarflow {
namespace services {

class SynchronizedResourcesServiceImpl;
class AvaFlow_API SynchronizedResourcesService : public IService {
public:
  DECLARE_RTTI

  DECLARE_PROPERTY_TYPE(SamplingRate, int, );
  DECLARE_SETGET_PROP(SamplingRate, int);

protected:
  SynchronizedResourcesServiceImpl *_impl;

public:
  SynchronizedResourcesService();
  virtual ~SynchronizedResourcesService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  virtual void LoadSettings(const JSONObject &settings);
};

MAKE_SERVICE_FACTORY(SynchronizedResourcesService, "SynchronizedResources")

} // namespace services
} // namespace avatarflow

#endif //__SYNCHRONIZEDRESOURCESSERVICE__