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

#ifndef __IDELEGATE__
#define __IDELEGATE__

#include <list>

#include "IDelegateEvent.h"
#include "atypes.h"

namespace utils {

#define DELEG_PCOUNT 1
#define DELEG_PARAMS Param1 p1
#define DELEG_PARAMS_T class Param1
#define DELEG_ARGS p1
#define DELEG_ARGS_T Param1

#include "IDelegate_implement.h"

#undef DELEG_PCOUNT
#undef DELEG_PARAMS
#undef DELEG_PARAMS_T
#undef DELEG_ARGS
#undef DELEG_ARGS_T

#define DELEG_PCOUNT 2
#define DELEG_PARAMS Param1 p1, Param2 p2
#define DELEG_PARAMS_T class Param1, class Param2
#define DELEG_ARGS p1, p2
#define DELEG_ARGS_T Param1, Param2

#include "IDelegate_implement.h"

#undef DELEG_PCOUNT
#undef DELEG_PARAMS
#undef DELEG_PARAMS_T
#undef DELEG_ARGS
#undef DELEG_ARGS_T

#define DELEG_PCOUNT 3
#define DELEG_PARAMS Param1 p1, Param2 p2, Param3 p3
#define DELEG_PARAMS_T class Param1, class Param2, class Param3
#define DELEG_ARGS p1, p2, p3
#define DELEG_ARGS_T Param1, Param2, Param3

#include "IDelegate_implement.h"

#undef DELEG_PCOUNT
#undef DELEG_PARAMS
#undef DELEG_PARAMS_T
#undef DELEG_ARGS
#undef DELEG_ARGS_T

#undef DELEG_PCOUNT

#define DELEG_PARAMS Param1 p1, Param2 p2, Param3 p3, Param4 p4
#define DELEG_PARAMS_T class Param1, class Param2, class Param3, class Param4
#define DELEG_ARGS p1, p2, p3, p4
#define DELEG_ARGS_T Param1, Param2, Param3, Param4

#include "IDelegate_implement.h"

#undef DELEG_PCOUNT
#undef DELEG_PARAMS
#undef DELEG_PARAMS_T
#undef DELEG_ARGS
#undef DELEG_ARGS_T

}  // namespace utils

#endif
