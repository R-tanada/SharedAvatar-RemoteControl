

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

#ifndef __CoreAvatarOS__
#define __CoreAvatarOS__

#include "../IAvatarOS.h"
#include "../OSCapabilities.h"
#include "../avataros_config.h"

namespace avatarOS {
class AvaOS_API CoreAvatarOS : public IAvatarOS {
private:
  /* data */

  OSCapabilities _caps;

  std::string _appPath;

public:
  CoreAvatarOS(const std::string &appPath = "");
  ~CoreAvatarOS();

  virtual std::string GetOSName() { return "Aeeon UP2"; }
  virtual const OSCapabilities &GetCapabilities() { return _caps; }

  const std::string &GetApplicationDirectory() { return _appPath; }
  virtual std::string GetModulePath();

  virtual void SetEnv(const std::string &name, const std::string &value,
                      bool overwrite = true);
  virtual std::string GetEnv(const std::string &name);

  void SetApplicationPath(const std::string &path);

  virtual void Shutdown();

#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
  // iOS device
#elif TARGET_OS_MAC
  // Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif

#endif // DEBUG

  std::string
  GetGStreamerEncoder(const std::string &codec,
                      const std::map<std::string, std::string> &encoderValues,
                      std::map<std::string, std::string> &output);
  std::string
  GetGStreamerDecoder(const std::string &codec,
                      const std::map<std::string, std::string> &encoderValues,
                      std::map<std::string, std::string> &output);

  virtual std::string GetOSType();
  virtual std::string GetOSVersion();
};

} // namespace avatarOS

#endif //__CoreAvatarOS__
