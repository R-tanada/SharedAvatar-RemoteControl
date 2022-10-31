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

#ifndef __AudioSourceSinkElement__
#define __AudioSourceSinkElement__

#include "flow/avatarflow_config.h"
#include <utils/JSONUtils.h>

namespace avatarflow {
namespace media {

class IAudioSourceDevice;
class IAudioSinkDevice;
class GstPipeline;

class AvaFlow_API AudioSourceSinkElement {
  IAudioSourceDevice *_source;
  IAudioSinkDevice *_sink;
  GstPipeline *_pipeline;

public:
  AudioSourceSinkElement();
  virtual ~AudioSourceSinkElement();

  IAudioSourceDevice *GetSource() { return _source; }
  IAudioSinkDevice *GetSink() { return _sink; }

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

#endif //__AudioSourceSinkElement__