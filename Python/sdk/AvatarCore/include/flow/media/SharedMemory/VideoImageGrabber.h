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

#include <utils/ListenerContainer.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class IVideoGrabber;
class VideoFrame;
class VideoImageGrabber;

class IVideoImageGrabberListener {
public:
  virtual ~IVideoImageGrabberListener() {}
  virtual void VideoGrabber_OnNewFrame(VideoImageGrabber *g,
                                       const VideoFrame *frame) {}
};

class VideoImageGrabberImpl;
class AvaFlow_API VideoImageGrabber
    : public utils::ListenerContainer<IVideoImageGrabberListener *> {
public:
  DECLARE_FIRE_METHOD(VideoGrabber_OnNewFrame,
                      (VideoImageGrabber * g, const VideoFrame *frame),
                      (g, frame));

protected:
  VideoImageGrabberImpl *_impl;

public:
  VideoImageGrabber(/* args */);
  virtual ~VideoImageGrabber();

  void AttachGrabber(IVideoGrabber *sink);

  bool Start(int sleepMS = 30);
  bool Stop();
};

} // namespace media
} // namespace avatarflow

#endif //__VIDEOSINKIMAGEGRABBER__