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

#ifndef __CloudPerfSERVICE__
#define __CloudPerfSERVICE__

#include "IService.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace services {

class CloudPerfServiceImpl;

class AvaFlow_API CloudPerfService : public IService {
protected:
  friend class CloudPerfServiceImpl;

public:
  static const utils::GUID PATH_CloudPerfInit;
  static const utils::GUID PATH_CloudPerfReport;

private:
  CloudPerfServiceImpl *_impl;

public:
  DECLARE_RTTI
  DECLARE_PROPERTY_TYPE(TestFile, std::string, );
  DECLARE_PROPERTY_TYPE(SamplingTime, int, )
  DECLARE_PROPERTY_TYPE(QualityMode, std::string, )
  DECLARE_PROPERTY_TYPE(Width, int, )
  DECLARE_PROPERTY_TYPE(Height, int, )
  DECLARE_PROPERTY_TYPE(Framerate, int, )
  DECLARE_PROPERTY_TYPE(Bitrate, int, )
  DECLARE_PROPERTY_TYPE(MinStreams, int, )
  DECLARE_PROPERTY_TYPE(MaxStreams, int, )
  DECLARE_PROPERTY_TYPE(TestsPerStream, int, )
  DECLARE_PROPERTY_TYPE(PeerID, int, )
  DECLARE_PROPERTY_TYPE(TargetResource, std::string, )
  DECLARE_PROPERTY_TYPE(Region, std::string, )
  DECLARE_PROPERTY_TYPE(DebugMode, bool, );

  DECLARE_SETGET_PROP(TestFile, std::string);
  DECLARE_SETGET_PROP(SamplingTime, int);
  DECLARE_SETGET_PROP(QualityMode, std::string);
  DECLARE_SETGET_PROP(Width, int);
  DECLARE_SETGET_PROP(Height, int);
  DECLARE_SETGET_PROP(Framerate, int);
  DECLARE_SETGET_PROP(Bitrate, int);
  DECLARE_SETGET_PROP(MinStreams, int);
  DECLARE_SETGET_PROP(MaxStreams, int);
  DECLARE_SETGET_PROP(TestsPerStream, int);
  DECLARE_SETGET_PROP(PeerID, int);
  DECLARE_SETGET_PROP(TargetResource, std::string);
  DECLARE_SETGET_PROP(Region, std::string);
  DECLARE_SETGET_PROP(DebugMode, bool);

public:
  CloudPerfService(/* args */);
  virtual ~CloudPerfService();

  virtual bool Init(ServiceContext *context);
  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();
  virtual void Update(float dt);

  virtual void RunTests();

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
};
MAKE_SERVICE_FACTORY(CloudPerfService, "CloudPerf")

} // namespace services
} // namespace avatarflow

#endif //__CloudPerfSERVICE__