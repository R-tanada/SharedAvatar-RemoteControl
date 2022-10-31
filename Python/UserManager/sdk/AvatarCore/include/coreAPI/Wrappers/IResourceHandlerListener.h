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

#ifndef __IRESOURCEHANDLERLISTENER__
#define __IRESOURCEHANDLERLISTENER__

#include <flow/services/IServiceSubscriber.h>

#include <string>

namespace avatarflow {
namespace media {
class AudioFrame;
class DataFrame;
class VideoFrame;
} // namespace media
} // namespace avatarflow

class CoreResourceHandler;

class IResourceHandlerListener {
public:
  virtual ~IResourceHandlerListener() {}
  virtual void OnVideoFrame(CoreResourceHandler *self,
                            const std::string &resource,
                            avatarflow::media::VideoFrame *frame) {}
  virtual void OnAudioFrame(CoreResourceHandler *self,
                            const std::string &resource,
                            avatarflow::media::AudioFrame *frame) {}
  virtual void OnDataFrame(CoreResourceHandler *self,
                           const std::string &resource,
                           const std::string &channel,
                           avatarflow::media::DataFrame *frame) {}
};

class ICoreAPIWrapperListener {
public:
  virtual ~ICoreAPIWrapperListener() {}
  virtual void OnAPIClosing() {}
};

class ICoreAPIServiceSubscriber
    : public avatarflow::services::IServiceSubscriber {
public:
  std::string name;
  std::string path;
  ICoreAPIServiceSubscriber(const std::string &n, const std::string &p)
      : name(n), path(p) {}
  virtual ~ICoreAPIServiceSubscriber() {}
};

#endif //__IAPPSINKCALLBACK__