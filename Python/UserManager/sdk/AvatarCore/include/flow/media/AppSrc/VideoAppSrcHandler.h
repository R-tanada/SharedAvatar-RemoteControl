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

#ifndef __VIDEOAPPSRCHANDLER__
#define __VIDEOAPPSRCHANDLER__

#include "IAppSrcHandler.h"

namespace avatarflow {
namespace media {

class IVideoGrabber;
class GstPipeline;
class VideoAppSrcHandlerImpl;
class AvaFlow_API VideoAppSrcHandler : public IAppSrcHandler {
private:
  VideoAppSrcHandlerImpl *_impl;

public:
  VideoAppSrcHandler(const std::string &name);
  virtual ~VideoAppSrcHandler();

  virtual void SetSrc(void *s, GstPipeline *pipeline, unsigned int rate,
                      bool pullMode = false);

  virtual void AttachVideoGrabber(IVideoGrabber *g);
  void Stop();

  bool IsStarted();
};

} // namespace media

} // namespace avatarflow

#endif //__VIDEOAPPSRCHANDLER__