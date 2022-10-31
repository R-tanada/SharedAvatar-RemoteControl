
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

#ifndef __AVATAROS_CONFIG__
#define __AVATAROS_CONFIG__

#if (defined(_WIN32) || defined(_WIN64))
#ifdef AvaOS_EXPORTS
#define AvaOS_API __declspec(dllexport)
#else
#define AvaOS_API __declspec(dllimport)
#endif
#else
#define AvaOS_API
#endif

#endif  //__AVATAROS_CONFIG__