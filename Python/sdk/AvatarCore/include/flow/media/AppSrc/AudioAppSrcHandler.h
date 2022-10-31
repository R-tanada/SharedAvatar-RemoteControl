
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

#ifndef __AUDIOAPPSRCHANDLER__
#define __AUDIOAPPSRCHANDLER__

#include "IAppSrcHandler.h"

namespace avatarflow {
namespace media {

class IAudioGrabber;
class AudioAppSrcHandlerImpl;
class AvaFlow_API AudioAppSrcHandler : public IAppSrcHandler {
private:
  AudioAppSrcHandlerImpl *_impl;

public:
  AudioAppSrcHandler(const std::string &name);
  ~AudioAppSrcHandler();

  virtual void SetSrc(void *s, GstPipeline *pipeline, unsigned int samplerate,
                      bool pullMode = false);

  virtual void AttachAudioGrabber(IAudioGrabber *g);
  void Stop();

  bool IsStarted();
};

} // namespace media
} // namespace avatarflow

#endif //__AUDIOAPPSRCHANDLER__
