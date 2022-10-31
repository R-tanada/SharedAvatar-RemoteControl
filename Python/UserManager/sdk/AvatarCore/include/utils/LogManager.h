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

#ifndef __LOGMANAGER__
#define __LOGMANAGER__

#include <iostream>
#include <list>
#include <mutex>
#include <sstream>

#include "ILogManager.h"

namespace utils {

class AvaUtils_API ConsoleLogDevice : public ILogDevice {
  std::string _prefix;

public:
  ConsoleLogDevice(const std::string &prefix = "") : _prefix(prefix) {}
  std::string GetTimeStamp();

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level, int vl);
  virtual bool Start();
  virtual void Close();

  virtual void Flush();
};

class AvaUtils_API LogManager : public ILogManager {
protected:
  typedef std::list<ILogDevice *> LogDeviceList;
  LogDeviceList m_loggers;
  std::mutex _logMutex;

public:
  LogManager();

  virtual ~LogManager();
  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level = ELL_INFO, int verbose = EVL_Normal);
  virtual void Logf(const utils::GUID &category, ELogLevel level, int vl,
                    const char *fmt, ...);
  virtual void AddLogDevice(ILogDevice *logger);
  virtual void RemoveLogDevice(ILogDevice *logger);
};
} // namespace utils

#endif //__LOGMANAGER__