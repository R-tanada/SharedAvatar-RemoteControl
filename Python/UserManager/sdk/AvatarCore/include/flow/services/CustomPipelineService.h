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

#ifndef __CUSTOMPIPELINESERVICE__
#define __CUSTOMPIPELINESERVICE__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace services {

class CustomPipelineServiceImpl;
class AvaFlow_API CustomPipelineService : public IService {
public:
  DECLARE_RTTI

protected:
  CustomPipelineServiceImpl *_impl;

  DECLARE_PROPERTY_TYPE(Pipeline, std::string, );
  DECLARE_SETGET_PROP(Pipeline, std::string);

public:
  CustomPipelineService(/* args */);
  virtual ~CustomPipelineService();

    virtual bool StartService();
    virtual bool StopService();
    virtual void Update(float dt);

  virtual bool IsRunning();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(CustomPipelineService, "CustomPipeline")

} // namespace services
} // namespace avatarflow

#endif //__CUSTOMPIPELINESERVICE__