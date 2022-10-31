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

#ifndef __ILogDevice__
#define __ILogDevice__

#include <string>

#include "GUID.h"
#include "avatarutils_config.h"
#include "base.h"

namespace utils {

// https://datatracker.ietf.org/doc/html/rfc5424 P.10
enum ELogLevel {
  ELL_EMERGENCY = 0,
  ELL_ALERT = 1,
  ELL_CRITICAL = 2,
  ELL_ERROR = 3,
  ELL_WARNING = 4,
  ELL_NOTICE = 5,
  ELL_INFO = 6,
  ELL_DEBUG = 7,
};
enum EVerbosLevel { EVL_Critical = 0, EVL_Normal = 1, EVL_Heavy = 2 };

class AvaUtils_API ILogDevice {
private:
  /* data */

protected:
  static const std::string &GetLogLevelString(ELogLevel l) {
    static std::string ELogLevelStr[] = {"Emergency", "Alert",   "Critical",
                                         "Error",     "Warning", "Notice",
                                         "Info",      "Debug"};
    return ELogLevelStr[l];
  }

public:
  ILogDevice() {}
  virtual ~ILogDevice() {}

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level, int verbosity) = 0;

  virtual bool Start() = 0;
  virtual void Close() = 0;

  virtual void Flush() = 0;
};

MakeSharedPtr(ILogDevice)

} // namespace utils

#endif //__ILogDevice__