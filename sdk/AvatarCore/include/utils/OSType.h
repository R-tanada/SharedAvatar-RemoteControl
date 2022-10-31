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

#ifndef __OSTYPE__
#define __OSTYPE__

#if defined(_WIN32) || defined(_WIN64)
#define __WINDOWS__

#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
// iOS device
#define __iOS__
// Other kinds of Mac OS
#else
#define __MACOSX__
#endif

#elif __linux__
#define __LINUX__
#elif __FreeBSD__
#elif __unix || __unix__
#define __UNIX__
#else

#endif

#if defined(__iOS__) || defined(__ANDROID__)
#define __MOBILE__
#endif

#endif  //__OSTYPE__
