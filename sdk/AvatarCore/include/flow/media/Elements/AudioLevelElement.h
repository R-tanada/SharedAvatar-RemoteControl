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

#ifndef __AUDIOLEVELELEMENT__
#define __AUDIOLEVELELEMENT__

#include "flow/avatarflow_config.h"
#include "flow/media/GStreamer/GstPipelineHandler.h"
#include "utils/GUID.h"

namespace avatarflow {
namespace media {

class GstPipeline;
class AudioLevelElement;

class IAudioLevelListener {
public:
  virtual ~IAudioLevelListener() {}
  virtual void OnAudioLevelStatsUpdated(AudioLevelElement *e) {}
};

class AvaFlow_API AudioLevelElement
    : public IPipelineListener,
      public utils::ListenerContainer<IAudioLevelListener *> {
private:
  void *_elem;
  std::string _name;
  GstPipeline *_pipeline;

  struct ChannelInfo {
    ChannelInfo() {
      rms = 0;
      peak = 0;
      decay = 0;
      rms_normalized = 0;
    }
    float rms, peak, decay, rms_normalized;
  };

  std::vector<ChannelInfo> _channels;

  DECLARE_FIRE_METHOD(OnAudioLevelStatsUpdated, (AudioLevelElement * e), (e));

public:
  static const utils::GUID PATH_AudioStatistics;
  static const utils::GUID PATH_EnableAudioStatistics;

public:
  AudioLevelElement(/* args */);
  virtual ~AudioLevelElement();

  float GetRMS(int channel, bool normalized);
  float GetPeak(int channel);
  float GetDecay(int channel);
  int GetChannelsCount() { return _channels.size(); }

  std::string BuildPipeline(const std::string &prefix);

  virtual void LinkWithPipeline(GstPipeline *pipeline);

  virtual void OnPipelineElementMessage(GstPipelineHandler *p,
                                        const std::string &elem, void *msg);
};

} // namespace media
} // namespace avatarflow

#endif //__AUDIOLEVELELEMENT__