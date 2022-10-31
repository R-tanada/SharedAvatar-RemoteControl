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

#ifndef __AVATARUTILS_CONFIG__
#define __AVATARUTILS_CONFIG__

#if (defined(_WIN32) || defined(_WIN64))
#ifdef AvaUTILS_EXPORTS
#define AvaUtils_API __declspec(dllexport)
#else
#define AvaUtils_API __declspec(dllimport)
#endif
#else
#define AvaUtils_API __attribute__((visibility("default")))
#define AvaUtils_StaticBuild
#endif

#endif  //__AVATARUTILS_CONFIG__