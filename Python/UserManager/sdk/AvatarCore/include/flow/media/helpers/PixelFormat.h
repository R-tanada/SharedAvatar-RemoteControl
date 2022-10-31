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

#ifndef __PIXELFORMAT__
#define __PIXELFORMAT__

#include "flow/avatarflow_config.h"
#include "utils/atypes.h"

namespace avatarflow {
namespace media {

enum PixelFormat {
    PF_ALPHA,  // Single channel
    PF_I420,
    PF_YUY2,
    PF_RGB,
    PF_RGBA,
    PF_BGR,
    PF_BGRA,
    PF_xRGB,
    PF_xBGR,
    PF_NV12,

    PF_UNKOWN
};

enum PixelFlags {
    EPixelFlags_HasAlpha = BIT(0),
    EPixelFlags_Float = BIT(1),
    EPixelFlags_Depth = BIT(2),
    EPixelFlags_Luminance = BIT(3),
    EPixelFlags_Compressed = BIT(4),
    EPixelFlags_NativeEndian = BIT(5)
};

struct PixelDescriptor {
    std::string name;
    PixelFormat type;

    utils::uchar size;  // element Size in bytes
    utils::uint flags;  // see PixelFlags
    utils::uchar componentsCount;
    float imageScaler;
};

class AvaFlow_API PixelUtils {
   public:
    static const PixelDescriptor& GetPixelDescriptor(PixelFormat f);

    static std::string GetGstFormat(PixelFormat fmt);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__PIXELFORMAT__