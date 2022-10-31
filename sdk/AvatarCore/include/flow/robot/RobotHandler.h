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

#ifndef __ROBOTHANDLER__
#define __ROBOTHANDLER__

#include <utils/MovingAverage.h>

#include "flow/avatarflow_config.h"
#include "flow/robot/RobotProtocol.h"
#include "flow/services/IAvatarBotService.h"
#include "flow/services/IServiceSubscriber.h"
#include "utils/point2D.h"

namespace avatarflow {
namespace robot {

class AvaFlow_API ExtRobotMessages : public RobotMessages {
public:
  DECLARE_ROBOT_MESSAGE(AutoPark)
};

class RobotHandler;

class IRobotHandlerListener {
public:
  virtual ~IRobotHandlerListener() {}
  virtual void OnBatteryChanged(RobotHandler *r, float level, bool charging) {}
  virtual void OnLIDARData(RobotHandler *r, const utils::byte *lidar,
                           int count) {}
  virtual void OnWheelsData(RobotHandler *r, int left, int right) {}
  virtual void OnAutoPark(RobotHandler *r) {}

  virtual void OnMessage(RobotHandler *robot,
                         const robot::RobotControlMessage &msg) {}
};

class AvaFlow_API RobotHandler
    : public robot::RobotProtocol,
      public services::IServiceSubscriber,
      public utils::ListenerContainer<IRobotHandlerListener *> {
protected:
  DECLARE_FIRE_METHOD(OnBatteryChanged,
                      (RobotHandler * r, float level, bool charging),
                      (r, level, charging));
  DECLARE_FIRE_METHOD(OnLIDARData,
                      (RobotHandler * r, const utils::byte *lidar, int count),
                      (r, lidar, count));
  DECLARE_FIRE_METHOD(OnWheelsData, (RobotHandler * r, int left, int right),
                      (r, left, right));
  DECLARE_FIRE_METHOD(OnAutoPark, (RobotHandler * r), (r));
  DECLARE_FIRE_METHOD(OnMessage,
                      (RobotHandler * r, const robot::RobotControlMessage &msg),
                      (r, msg));

  utils::GUID _ID;

public:
  RobotHandler();
  virtual void OnMessageArrived(const robot::RobotControlMessage &msg);

  void Subscriber_OnData(const utils::GUID &serviceName,
                         const utils::GUID &path, const void *data,
                         utils::uint length, utils::uint address,
                         bool forwarded);

  void Start();
  void Stop();

public:
  // control signals
  void SetWheels(int left, int right);
  void AutoPark();

public:
  // feedback signals
  void SetLIDARData(utils::byte *data, int len);
  void SetBatteryLevel(utils::byte level, bool charging);
};

} // namespace robot
} // namespace avatarflow

#endif //__ROBOTHANDLER__