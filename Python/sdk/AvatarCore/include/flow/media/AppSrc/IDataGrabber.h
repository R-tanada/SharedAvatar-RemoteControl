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

#ifndef __IDATAGRABBER__
#define __IDATAGRABBER__

#include <utils/atypes.h>

#include "flow/media/Resources/DataFrame.h"

namespace avatarflow {
namespace media {

class IDataGrabber {
protected:
public:
  IDataGrabber() {}
  virtual ~IDataGrabber() {}

  virtual bool GrabFrame() = 0;
  virtual bool HasNewFrame() const = 0;
  virtual utils::uint GetFrameLength() const = 0;
  virtual const DataFrame *GetLastFrame() const = 0;

  virtual void Lock() {}
  virtual void Unlock() {}
};

} // namespace media
} // namespace avatarflow

#endif //__IDATAGRABBER__