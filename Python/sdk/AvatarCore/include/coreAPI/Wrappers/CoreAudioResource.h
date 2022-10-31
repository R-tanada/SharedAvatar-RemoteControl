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

#ifndef __COREAudioRESOURCE__
#define __COREAudioRESOURCE__

#include "IResourceHandlerListener.h"
#include "coreAPI/avatarcore_config.h"

class CoreResourceHandler;

class AvaCore_API CoreAudioResource : public IResourceHandlerListener {
public:
  CoreAudioResource();
  ~CoreAudioResource();

  virtual bool Start(const std::string &resource, int timeout);
  virtual bool Stop();

  virtual void Destroy();

  // needs to be implemented by the wrapper
  virtual void OnAudioFrame(CoreResourceHandler *self,
                            const std::string &resourceName,
                            avatarflow::media::AudioFrame *frame) {}

  virtual bool WriteFrame(avatarflow::media::AudioFrame *frame,
                          const std::string &resourceName);

protected:
  CoreResourceHandler *player;
};

#endif //__COREAudioRESOURCE__