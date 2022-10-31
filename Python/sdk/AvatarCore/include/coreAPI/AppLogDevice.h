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

#ifndef __APPLOGDEVICE__
#define __APPLOGDEVICE__

#include <utils/ILogDevice.h>

#include "avatarcore_config.h"

namespace utils {

class AvaCore_API AppLogDevice : public ILogDevice {
public:
  typedef void (*LoggerFunction)(const utils::GUID &category,
                                 const std::string &msg, ELogLevel level,
                                 int vl);

private:
  LoggerFunction _func;

public:
  AppLogDevice(LoggerFunction f) : _func(f) {}
  ~AppLogDevice() {}

  void AttachLogger(LoggerFunction func) { _func = func; }

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level, int vl);

  virtual bool Start() { return true; }
  virtual void Close() {}

  virtual void Flush() {}
};

} // namespace utils

#endif //__APPLOGDEVICE__