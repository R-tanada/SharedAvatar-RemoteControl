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

#ifndef __RESOURCEHELPER__
#define __RESOURCEHELPER__

#include <string>

#include "avatarcore_config.h"

namespace avatarflow {

class AvaCore_API ResourceHelper {
private:
  /* data */
public:
  static std::string LocalVideo();
  static std::string RemoteFrontVideo();

  // avatar core hardware default resources
  static std::string LeftCamera();
  static std::string RightCamera();
  static std::string AutoFocusCamera();
  static std::string BottomCamera();

  static std::string LocalAudio();
  static std::string RemoteAudio();

  static std::string LocalData();
  static std::string RemoteData();
};

} // namespace avatarflow

#endif //__RESOURCEHELPER__