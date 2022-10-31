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

#ifndef __AUDIOINTERRUPTERELEMENT__
#define __AUDIOINTERRUPTERELEMENT__

#include <gst/audio/audio.h>
#include <string>
#include <utils/ListenerContainer.h>

#include "flow/avatarflow_config.h"
#include "flow/media/GStreamer/GstMyListener.h"


namespace avatarflow {
namespace media {

class GstPipeline;
class AudioInterrupterElement;
class AudioInterrupterElementImpl;
class IAudioInterrupterListener {
public:
  virtual void OnNewAudioBufferArrived(AudioInterrupterElement *e,
                                       GstCaps *caps, const GstAudioInfo &info,
                                       GstBuffer *buffer) {}
};

/**
 * @brief This class will interrupt a media pipeline so you can manipulate an
 * image frame when new buffers reaches it
 *
 */
class AvaFlow_API AudioInterrupterElement
    : public utils::ListenerContainer<IAudioInterrupterListener *> {
private:
  AudioInterrupterElementImpl *_impl;

public:
  DECLARE_FIRE_METHOD(OnNewAudioBufferArrived,
                      (AudioInterrupterElement * e, GstCaps *caps,
                       const GstAudioInfo &info, GstBuffer *buffer),
                      (e, caps, info, buffer));

public:
  AudioInterrupterElement(/* args */);
  virtual ~AudioInterrupterElement();

  std::string BuildPipeline(const std::string &prefix);

  virtual void LinkWithPipeline(GstPipeline *pipeline);

  GstMyListener *GetListener();
};

} // namespace media

} // namespace avatarflow

#endif //__AUDIOINTERRUPTERELEMENT__