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

#ifndef __FPSCALC__
#define __FPSCALC__

#include "avataros_config.h"
#include "utils/atypes.h"

namespace avatarOS {

//! FPS(Fram per second)

class FPSCalcImpl;
class AvaOS_API FPSCalc {
  FPSCalcImpl *_impl;

public:
  FPSCalc();
  virtual ~FPSCalc();
  float getFPS();
  float dt();

  void resetTime();

  utils::uint CounterID();

  //! register a new frame
  void regFrame();
};

} // namespace avatarOS

#endif
