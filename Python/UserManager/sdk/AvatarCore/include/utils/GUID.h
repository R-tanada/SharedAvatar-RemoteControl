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

#ifndef __GUID__
#define __GUID__

#include <string>

#include "CRC.h"
#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {
// Helper class to create a GUID as a unique identifier for a string.
// Uses CRC algorithm to hash the string value
class AvaUtils_API GUID {
   protected:
    std::string _string;
    uint _crc;

   public:
    GUID() : _crc(0) {}

    GUID(const GUID &o) : _string(o._string), _crc(o._crc) {}

    GUID(const std::string &str) { *this = str; }
    GUID(uint crc) { _crc = crc; }
    GUID(const char *str) { *this = std::string(str); }
    virtual ~GUID() {}

    const std::string &GetString() const { return _string; }

    GUID &operator=(const std::string &str) {
        _string = str;
        _crc = CRC::CalcCRC(str.c_str(), (uint)str.length());
        return *this;
    }

    inline bool operator<(const GUID &o) const { return _crc < o._crc; }

    inline bool operator==(const GUID &o) const { return _crc == o._crc; }
    inline bool operator!=(const GUID &o) const { return _crc != o._crc; }
    inline uint ID() const { return _crc; }
};

}  // namespace utils

#endif  //__GUID__