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

#ifndef __AVATARCORE_CONFIG__
#define __AVATARCORE_CONFIG__

#if (defined(_WIN32) || defined(_WIN64))
#ifdef AvaCore_EXPORTS
#define AvaCore_API __declspec(dllexport)
#else
#define AvaCore_API __declspec(dllimport)
#endif
#else
#define AvaCore_API
#endif

#endif  //__AVATARCORE_CONFIG__