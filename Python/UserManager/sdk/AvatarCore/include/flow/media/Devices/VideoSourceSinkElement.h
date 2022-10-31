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

#ifndef __VideoSourceSinkElement__
#define __VideoSourceSinkElement__

#include "flow/avatarflow_config.h"
#include <utils/JSONUtils.h>

namespace avatarflow {
namespace media {

class IVideoSourceDevice;
class IVideoSinkDevice;
class GstPipeline;

class AvaFlow_API VideoSourceSinkElement {
  IVideoSourceDevice *_source;
  IVideoSinkDevice *_sink;
  GstPipeline *_pipeline;

public:
  VideoSourceSinkElement();
  virtual ~VideoSourceSinkElement();

  IVideoSourceDevice *GetSource() { return _source; }
  IVideoSinkDevice *GetSink() { return _sink; }

  void Open();
  void Close();
  void Destroy();

  virtual void LinkWithPipeline(GstPipeline *pipeline);
  virtual void LoadSettings(const JSONObject &settings, bool recreateSource,
                            bool recreateSink);
  virtual void OnSettingsUpdated(const JSONObject &settings);

private:
};

} // namespace media
} // namespace avatarflow

#endif //__VideoSourceSinkElement__