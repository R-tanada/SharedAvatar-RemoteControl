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

#ifndef __IAVATAROS__
#define __IAVATAROS__

#include <utils/ISingleton.h>
#include <utils/atypes.h>

#include <map>

#include "avataros_config.h"

namespace avatarOS {

class OSCapabilities;

class IAvatarOSImpl;

class AvaOS_API IAvatarOS : public utils::ISingleton<IAvatarOS> {
private:
protected:
  IAvatarOSImpl *_impl;

public:
  IAvatarOS();
  virtual ~IAvatarOS();

  virtual std::string GetOSName() = 0;
  virtual const OSCapabilities &GetCapabilities() = 0;

  virtual void Shutdown() = 0;

  virtual void SetApplicationPath(const std::string &path) = 0;
  virtual const std::string &GetApplicationDirectory() = 0;

  virtual std::string GetModulePath() = 0;

  virtual void SetEnv(const std::string &name, const std::string &value,
                      bool overwrite = true) = 0;
  virtual std::string GetEnv(const std::string &name) = 0;

  virtual unsigned long long GetTimeMillisecond();
  virtual unsigned long long GetTimeNanosecond();

  virtual void SleepMillisecond(utils::ulong ms);
  virtual void SleepNanosecond(utils::ulong ms);

  virtual std::string
  GetGStreamerEncoder(const std::string &codec,
                      const std::map<std::string, std::string> &encoderValues,
                      std::map<std::string, std::string> &output) = 0;

  virtual std::string
  GetGStreamerDecoder(const std::string &codec,
                      const std::map<std::string, std::string> &encoderValues,
                      std::map<std::string, std::string> &output) = 0;

  static int GetAvailableNetworkPort(); // ask the OS to get a free port

  virtual std::string GetOSType() = 0;
  virtual std::string GetOSVersion() = 0;

  // creates a valid path correcting \ and / depending on the OS, and ends the
  // string with the correct character e.g. Windows: C:/xx --> C:/xx/
  static std::string CorrectFolderPath(const std::string &path);
};

#define gAvatarOS avatarOS::IAvatarOS::getInstance()

} // namespace avatarOS

#endif //__IAVATAROS__