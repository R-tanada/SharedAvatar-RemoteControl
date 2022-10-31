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

#ifndef __COREVIDEORESOURCE__
#define __COREVIDEORESOURCE__

#include "coreAPI/avatarcore_config.h"
#include <OS/FPSCalc.h>

#include "IResourceHandlerListener.h"

class CoreResourceHandler;

class AvaCore_API CoreVideoResource : public IResourceHandlerListener {
public:
  CoreVideoResource();
  ~CoreVideoResource();

  virtual bool Start(const std::string &resource, int timeout);
  virtual bool Stop();

  virtual void Destroy();

  // needs to be implemented by the wrapper
  virtual void OnVideoFrame(CoreResourceHandler *self,
                            const std::string &resourceName,
                            avatarflow::media::VideoFrame *frame) {
    _fps.regFrame();
  }

  virtual bool WriteFrame(avatarflow::media::VideoFrame *frame,
                          const std::string &resourceName);

protected:
  CoreResourceHandler *player;
  avatarOS::FPSCalc _fps;
};

#endif //__COREVIDEORESOURCE__