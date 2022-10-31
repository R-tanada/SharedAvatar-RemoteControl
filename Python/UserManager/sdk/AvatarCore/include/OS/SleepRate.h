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

#ifndef __SLEEPRATE__
#define __SLEEPRATE__

#include "avataros_config.h"

namespace avatarOS {
class AvaOS_API SleepRate {
private:
  int _targetSleep;
  unsigned long long _lastTime;
  unsigned long long _sleepTime;

public:
  SleepRate(int ms);
  ~SleepRate();

  bool Sleep();
  bool Sleep(int ms);
};

} // namespace avatarOS

#endif //__SLEEPRATE__