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

#ifndef __IAVATARBOTSERVICE__
#define __IAVATARBOTSERVICE__

#include <utils/OSType.h>

#include "IService.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace services {

class IAvatarBotServiceImpl;

class AvaFlow_API IAvatarBotService : public IService {
public:
  DECLARE_RTTI

  class BatteryState {
  public:
    utils::ushort MinLevel;
    utils::ushort MaxLevel;

    std::vector<utils::ushort> cells;

    utils::ushort discargeCount;
    utils::ushort TimeToFull;  // mins
    utils::ushort TimeToEmpty; // mins
    utils::ushort CurrentLevel;
    bool IsCharging;

    BatteryState()
        : MinLevel(0), MaxLevel(100), discargeCount(0), TimeToEmpty(0),
          TimeToFull(0), CurrentLevel(0), IsCharging(false) {}

    float GetLevelNormalized() {
      return (float)(CurrentLevel - MinLevel) / (float)(MaxLevel - MinLevel);
    }
  };

  struct LidarPoint {
    LidarPoint() : angle(0), distance(0) {}
    LidarPoint(const LidarPoint &o) : angle(o.angle), distance(o.distance) {}
    LidarPoint(short a, unsigned short d) : angle(a), distance(d) {}

    short angle;
    unsigned short distance;
  };

  static const utils::GUID PATH_LIDARData;
  static const utils::GUID PATH_BatteryData;
  static const utils::GUID PATH_WheelsMovement;
  static const utils::GUID PATH_MotionDir; // Speed & Rotation
  static const utils::GUID PATH_NeckChange;
  static const utils::GUID PATH_StartLIDAR;
  static const utils::GUID PATH_StopLIDAR;
  static const utils::GUID PATH_AutoPark;
  static const utils::GUID PATH_FreezeControl;
  static const utils::GUID PATH_CustomCommand;
  static const utils::GUID PATH_ErrorStatus;
  static const utils::GUID PATH_EnableCollisionDetection;

  DECLARE_PROPERTY_SETGET(WheelsTimeout, utils::uint);
  DECLARE_PROPERTY_SETGET(WheelsSpeedIn, float);
  DECLARE_PROPERTY_SETGET(WheelsSpeedOut, float);
  DECLARE_PROPERTY_SETGET(RotationOmega, float);
  DECLARE_PROPERTY_SETGET(WheelSpeed, float);
  DECLARE_PROPERTY_SETGET(WheelBackwardSpeed, float);
  DECLARE_PROPERTY_SETGET(RotationSpeed, float);
  DECLARE_PROPERTY_SETGET(StartCollisionDistance, float);
  DECLARE_PROPERTY_SETGET(CollisionDistance, float);
  DECLARE_PROPERTY_SETGET(LIDARStartAngle, float);
  DECLARE_PROPERTY_SETGET(LIDARStopAngle, float);

private:
  IAvatarBotServiceImpl *_impl;

protected:
  friend class IAvatarBotServiceImpl;

  // Connect to the robot hardware
  virtual bool _StartBot() { return false; }
  // Disconnect from the robot hardware
  virtual bool _StopBot() { return false; }

  // Is the robot started?
  virtual bool _IsBotStarted() { return false; }

  // Control motion, values are arbiturary for now
  virtual void _SetMotionHeading(float speed, float rotation) {}
  // Set neck angles in degrees
  virtual void _SetNeckPosition(float tilt, float pan, float roll) {}

  // retrive battery information if available
  virtual BatteryState *_GetBatteryState() { return 0; }

  virtual void _OnLidarData(const std::vector<LidarPoint> &data);
  virtual void _OnBatteryState(const BatteryState &data);

  // send a custom command (CMD) and arguments (args)
  virtual void _SendCustomCommand(const std::string &cmd,
                                  const std::string &args) {}

  virtual void _EnableCollisionDetection(bool e) {}

  virtual void _StartLIDAR() {}
  virtual void _StopLIDAR() {}

  // callback from a separate thread to update the robot
  // dt: delta time of the update loop
  // time: current time stamp in correspondence to the update loop
  virtual void _OnUpdate(float dt, utils::uint time) {
  } // multi threaded update callback

  void _registerDefaultProps();

public:
  IAvatarBotService(const std::string &name);
  virtual ~IAvatarBotService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();
  virtual void Update(float dt);

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);

  static bool SendMoveWheels(const utils::GUID &sender, int left, int right);
  static bool SendMotionDir(const utils::GUID &sender, float speed,
                            float rotation);
  static bool SendNeckChange(const utils::GUID &sender, int tilt, int pan,
                             int roll);
  static bool SendStartLidar(const utils::GUID &sender);
  static bool SendStopLidar(const utils::GUID &sender);
  static bool SendStartAutoParking(const utils::GUID &sender);
  static bool SendCustomCommand(const utils::GUID &sender,
                                const std::string &cmd,
                                const std::string &args);

  void PublishSensors();
  void PublishBattery();
};

} // namespace services
} // namespace avatarflow

#endif //__IAVATARBOTSERVICE__