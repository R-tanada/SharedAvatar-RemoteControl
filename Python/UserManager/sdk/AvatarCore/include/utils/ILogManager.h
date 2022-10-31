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

#ifndef __ILogManager__
#define __ILogManager__

#include <map>

#include "ILogDevice.h"
#include "ISingleton.h"
#include "StringConverter.h"

namespace utils {
class AvaUtils_API ILogManager : public ISingleton<ILogManager> {
private:
protected:
  utils::GUID _outputCategory;
  ELogLevel _outputLevel;
  int _outputVerbose;

  virtual void Output(const std::string &v) {
    Log(_outputCategory, v, _outputLevel, _outputVerbose);
  }

  int _maxVerbose;

  std::map<utils::GUID, bool> _categories;

public:
  ILogManager(/* args */)
      : _outputLevel(ELL_INFO), _outputVerbose(EVL_Critical),
        _maxVerbose(EVL_Normal) {}
  virtual ~ILogManager() {}
  virtual void AddLogDevice(ILogDevice *logger) = 0;
  virtual void RemoveLogDevice(ILogDevice *logger) = 0;

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level = ELL_INFO, int verbosity = EVL_Normal) = 0;
  virtual void Logf(const utils::GUID &category, ELogLevel level, int verbosity,
                    const char *fmt, ...) = 0;

  virtual void Flush() {}

  virtual void EnableCategory(const utils::GUID &category, bool enable) {
    _categories[category] = enable;
  }

  // by default all categories are enabled unless if specified not
  virtual bool IsCategoryEnabled(const utils::GUID &category) {
    const auto &it = _categories.find(category);
    if (it == _categories.end())
      return true;
    return it->second;
  }

  virtual void SetVerboseLevel(int level) { _maxVerbose = level; }

  virtual ILogManager &StartLog(const utils::GUID &category, ELogLevel level,
                                int verbosity = EVL_Normal) {
    _outputLevel = level;
    _outputVerbose = verbosity;
    _outputCategory = category;
    return *this;
  }

  // make sure to call StartLog before stream log your output
  template <class T> ILogManager &operator<<(const T &v) {
    Output(utils::StringConverter::toString(v));
    return *this;
  }
};

#define gLogManager utils::ILogManager::getInstance()

} // namespace utils

#endif //__ILogManager__