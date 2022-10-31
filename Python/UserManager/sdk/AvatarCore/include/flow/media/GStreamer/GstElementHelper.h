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

#ifndef __GSTELEMENTHELPER__
#define __GSTELEMENTHELPER__

#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {
class AvaFlow_API GstElementHelper {
   public:
    static void SetProperty(void* e, const char* prop, int val);
    static void SetProperty(void* e, const char* prop, float val);
    static void SetProperty(void* e, const char* prop, bool val);
    static void SetProperty(void* e, const char* prop, const char* val);

    static int GetPropertyInt(void* e, const char* prop);
    static float GetPropertyFloat(void* e, const char* prop);
    static bool GetPropertyBool(void* e, const char* prop);
    static std::string GetPropertyString(void* e, const char* prop);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__GSTELEMENTHELPER__