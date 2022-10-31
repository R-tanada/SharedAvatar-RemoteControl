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

#ifndef __CRC__
#define __CRC__

#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {
#define INITIAL_CRC_VAL 0xffffffff

class AvaUtils_API CRC {
   public:
    static uint CalcCRC(const char *buff, uint len,
                        uint crcVal = INITIAL_CRC_VAL);
    // static uint CalcCRC(IStream *stream, uint crcVal = INITIAL_CRC_VAL);
};

}  // namespace utils

#endif  //__CRC__