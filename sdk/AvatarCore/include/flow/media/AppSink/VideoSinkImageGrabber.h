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

#ifndef __VIDEOSINKIMAGEGRABBER__
#define __VIDEOSINKIMAGEGRABBER__

#include "flow/avatarflow_config.h"
#include <utils/ListenerContainer.h>

namespace avatarflow {
namespace media {

class VideoSinkImageGrabber;
class VideoAppSinkHandler;
class IAppSinkHandler;
class VideoSinkImageGrabberImpl;
class VideoFrame;

class IVideoSinkImageGrabberListener {
public:
  virtual ~IVideoSinkImageGrabberListener() {}
  virtual void VideoSink_OnNewFrame(VideoSinkImageGrabber *g,
                                    const VideoFrame *frame) {}
};

class AvaFlow_API VideoSinkImageGrabber
    : public utils::ListenerContainer<IVideoSinkImageGrabberListener *> {
public:
  DECLARE_FIRE_METHOD(VideoSink_OnNewFrame,
                      (VideoSinkImageGrabber * g, const VideoFrame *frame),
                      (g, frame));

private:
  VideoSinkImageGrabberImpl *_impl;

public:
  VideoSinkImageGrabber(/* args */);
  virtual ~VideoSinkImageGrabber();

  void AttachSink(VideoAppSinkHandler *sink);

  bool Start(bool triggerBsaed, int ms = 10);
  bool Stop();
};

} // namespace media
} // namespace avatarflow

#endif //__VIDEOSINKIMAGEGRABBER__