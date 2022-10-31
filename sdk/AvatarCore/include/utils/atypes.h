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

#ifndef __types__
#define __types__

#include <list>
#include <map>
#include <string>
#include <vector>

#define BIT(x) (1 << (x))

namespace utils {

    typedef std::uint8_t uchar;
    typedef signed char schar;

    typedef unsigned char byte;

    typedef std::uint32_t uint;
    typedef signed int sint;

    typedef std::uint16_t ushort;
    typedef signed short sshort;

    typedef std::uint64_t ulong;
    typedef unsigned long long ullong;
    typedef signed long slong;

    //typedef uchar utf8;
    //typedef uint utf32;

#ifndef PVOID
    typedef void* PVOID;
#endif

#ifdef UNICODE

    typedef wchar_t tchar;
#define aT(v) (v)

#else

    typedef char tchar;
#define aT(v) (v)

#endif

    const std::string EmptyString("");

}  // namespace utils

#endif  //__types__