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

#ifndef __GSTREAMERCORE__
#define __GSTREAMERCORE__
#include <utils/ISingleton.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {
class GStreamerCoreImpl;
class AvaFlow_API GStreamerCore : public utils::ISingleton<GStreamerCore> {
private:
  GStreamerCoreImpl *_impl;

public:
  GStreamerCore();
  ~GStreamerCore();

  bool Initialize(const std::string &pluginsPath = "");
  void Run(bool multithreaded = true);
  void Process();
  void Shutdown();

  void *GetGstContext();

  std::string GetVersion();
};

} // namespace media
} // namespace avatarflow

#endif //__GSTREAMERCORE__