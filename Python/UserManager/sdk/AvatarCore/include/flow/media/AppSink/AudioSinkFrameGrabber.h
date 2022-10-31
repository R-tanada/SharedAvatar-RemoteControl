

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

#ifndef __AUDIOSINKFRAMEGRABBER__
#define __AUDIOSINKFRAMEGRABBER__

#include "flow/avatarflow_config.h"
#include <utils/ListenerContainer.h>

namespace avatarflow {
namespace media {

class AudioSinkFrameGrabber;
class AudioAppSinkHandler;
class IAudioAppSink;
class AudioFrame;
class AudioSinkFrameGrabberImpl;

class IAudioSinkFrameGrabberListener {
public:
  virtual ~IAudioSinkFrameGrabberListener() {}
  virtual void AudioSink_OnNewFrame(AudioSinkFrameGrabber *g,
                                    const AudioFrame *frame) {}
};

class AvaFlow_API AudioSinkFrameGrabber
    : public utils::ListenerContainer<IAudioSinkFrameGrabberListener *> {
public:
  DECLARE_FIRE_METHOD(AudioSink_OnNewFrame,
                      (AudioSinkFrameGrabber * g, const AudioFrame *frame),
                      (g, frame));

private:
  AudioSinkFrameGrabberImpl *_impl;

public:
  AudioSinkFrameGrabber();
  ~AudioSinkFrameGrabber();
  void AttachSink(AudioAppSinkHandler *sink);

  bool Start(bool triggerBased);
  bool Stop();
};

} // namespace media

} // namespace avatarflow

#endif //__AUDIOSINKFRAMEGRABBER__