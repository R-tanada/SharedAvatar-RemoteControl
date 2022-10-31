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

#ifndef __FILELOGGERDEVICE__
#define __FILELOGGERDEVICE__

#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>

#include "ILogManager.h"

namespace utils {

class AvaUtils_API FileLoggerDevice : public ILogDevice {
  // std::ofstream _logfile;
  FILE *_logfile;
  std::string _path;
  std::mutex _logMutex;

public:
  FileLoggerDevice(const std::string &file, bool append);

  std::string GetTimeStamp();

  virtual void Log(const utils::GUID &category, const std::string &msg,
                   ELogLevel level, int verbosity);
  virtual bool Start();
  virtual void Close();

  virtual void Flush();
};

} // namespace utils

#endif //__FILELOGGERDEVICE__