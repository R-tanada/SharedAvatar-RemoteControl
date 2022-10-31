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

#ifndef __AudioSpeedElement__
#define __AudioSpeedElement__

#include "flow/avatarflow_config.h"
#include "flow/media/GStreamer/GstPipelineHandler.h"
#include "utils/GUID.h"
#include "utils/PropertySet.h"
#include "utils/Tween.h"
#include "utils/TypedProperty.h"

namespace avatarflow {
namespace media {

class GstPipeline;
class AvaFlow_API AudioSpeedElement : public IPipelineListener,
                                      public utils::PropertySet {
  DECLARE_RTTI
private:
  void *_queue;
  void *_speed;
  GstPipeline *_pipeline;
  std::string _prefix;

  float _lastSpeedFactor;

  utils::DTween _speedTween;

  bool _thresholdTriggered;

protected:
  bool _enabled;
  // max speed to be set
  float _maxSpeed;
  // omega value controlling the speed of change
  float _omega;
  // minimum threshould
  float _minThreshold;
  // turn off speed tuning when this value is reached
  float _offThreshold;
  // when buffer time exceeds this, the speed will be triggered
  float _triggerThreshold;

public:
  DECLARE_PROPERTY_TYPE(Enabled, bool, )
  DECLARE_PROPERTY_TYPE(MaxSpeed, float, )
  DECLARE_PROPERTY_TYPE(Omega, float, )
  DECLARE_PROPERTY_TYPE(MinThreshold, float, )
  DECLARE_PROPERTY_TYPE(OffThreshold, float, )
  DECLARE_PROPERTY_TYPE(TriggerThreshold, float, )

  DECLARE_SETGET_PROP(Enabled, bool);
  DECLARE_SETGET_PROP(MaxSpeed, float);
  DECLARE_SETGET_PROP(Omega, float);
  DECLARE_SETGET_PROP(MinThreshold, float);
  DECLARE_SETGET_PROP(OffThreshold, float);
  DECLARE_SETGET_PROP(TriggerThreshold, float);

public:
  AudioSpeedElement(/* args */);
  virtual ~AudioSpeedElement();

  std::string BuildPipeline(const std::string &prefix);

  virtual void LinkWithPipeline(GstPipeline *pipeline);

  void Update(float dt, int externalDuration);
};

} // namespace media
} // namespace avatarflow

#endif //__AudioSpeedElement__