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

#ifndef __LOGSUBSYSTEM__
#define __LOGSUBSYSTEM__

#include <list>

#include "ILogDevice.h"
#include "ILogManager.h"

namespace utils {

class AvaUtils_API LogSubsystem {
private:
protected:
  std::string m_systemType;
  std::list<ILogDevice *> m_logDevices;

public:
  LogSubsystem(const std::string &type);
  virtual ~LogSubsystem();

  const std::string &GetType() { return m_systemType; }

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level, int vl = EVL_Normal);

  virtual void AddLogDevice(ILogDevice *logger);
  virtual void RemoveLogDevice(ILogDevice *logger);
  virtual void ClearLogDevices();
};

#define DECLARE_LOGGER_SYSTEM(SystemName)                                      \
  class LoggerSystem##SystemName                                               \
      : public ISingleton<LoggerSystem##SystemName>,                           \
        public LogSubsystem {                                                  \
  public:                                                                      \
    LoggerSystem##SystemName() : LogSubsystem(mT(#SystemName)) {}              \
    virtual ~LoggerSystem##SystemName() {}                                     \
  };

DECLARE_LOGGER_SYSTEM(Generic)
#define gGenericLoggerSystem utils::LoggerSystemGeneric::getInstance()

} // namespace utils
#endif // LogSubsystem_h__
