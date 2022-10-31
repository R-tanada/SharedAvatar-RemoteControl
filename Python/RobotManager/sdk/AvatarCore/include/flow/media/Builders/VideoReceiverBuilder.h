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

#ifndef __VideoReceiverBuilder__
#define __VideoReceiverBuilder__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>
#include <utils/TypedProperty.h>
#include <utils/VariableManager.h>
#include <utils/atypes.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API VideoReceiverBuilder : public utils::PropertySet {
    DECLARE_RTTI
   private:
   public:
    enum EType {
        None,
        H264,
        H265,
        VP8,
        VP9,
    };
    std::string _encoder;

    EType _type;

   public:
    DECLARE_PROPERTY_TYPE(Encoder, std::string, );  // H264/VP8
    DECLARE_SETGET_PROP(Encoder, std::string);

   public:
    VideoReceiverBuilder();
    ~VideoReceiverBuilder();

    std::string BuildPipeline(bool addRtp = false, int payload = -1,
                              const std::string &prefix = "");

    std::string GetEncodingName();

    EType GetEncoderType() { return _type; }
};

}  // namespace media
}  // namespace avatarflow

#endif  //__VideoReceiverBuilder__