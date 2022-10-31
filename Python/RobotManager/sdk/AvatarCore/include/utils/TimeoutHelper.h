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

#ifndef __TIMEOUTHELPER__
#define __TIMEOUTHELPER__

#include "atypes.h"

namespace utils {
class TimeoutHelper {
private:
  utils::ullong _lastTime;
  utils::ullong _timeout;

public:
  TimeoutHelper(/* args */) {
    _lastTime = 0;
    _timeout = 0;
  }
  ~TimeoutHelper() {}

  void Init(utils::ullong timeout, utils::ullong now) {
    this->_timeout = timeout;
    _lastTime = now;
  }

  bool Check(utils::ullong now) {
    if ((long)(now - _lastTime) > _timeout) {
      _lastTime = now;
      return true;
    }
    return false;
  }
};

} // namespace utils

#endif //__TIMEOUTHELPER__