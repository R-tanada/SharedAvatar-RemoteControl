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

#ifndef __TimeTrigger__
#define __TimeTrigger__

#include "avataros_config.h"

namespace avatarOS {
class AvaOS_API TimeTrigger {
private:
  unsigned long long _triggerTime;
  unsigned long long _lastTime;

public:
  TimeTrigger(int ms);
  ~TimeTrigger();

  bool Check();
};

} // namespace avatarOS

#endif //__TimeTrigger__