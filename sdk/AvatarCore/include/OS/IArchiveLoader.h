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

#ifndef __IARCHIVELOADER__
#define __IARCHIVELOADER__

#include <string>

namespace avatarOS {

class IArchive;

class IArchiveLoader {
public:
  virtual ~IArchiveLoader(){};
  virtual IArchive *LoadArchive(const std::string &name) = 0;
  virtual std::string GetArchiveType() = 0;
  virtual bool CanLoadArchive(const std::string &name) = 0;
};

} // namespace avatarOS

#endif
