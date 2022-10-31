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

#ifndef __DataSenderBuilder__
#define __DataSenderBuilder__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>
#include <utils/TypedProperty.h>
#include <utils/atypes.h>
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {
class AvaFlow_API DataSenderBuilder : public utils::PropertySet {
    DECLARE_RTTI
   private:
   public:
    enum EType {
        None,
        BZip,
    };
    std::string _encoder;
    EType _type;

    DECLARE_PROPERTY_TYPE(Encoder, std::string, );
    DECLARE_SETGET_PROP(Encoder, std::string);

   public:
    DataSenderBuilder();
    ~DataSenderBuilder();

    std::string BuildPipeline(bool addRtp = false, int payload = -1,
                              const std::string &prefix = "");
    std::string GetRTPPayloader();
};

}  // namespace media

}  // namespace avatarflow

#endif  //__DataSenderBuilder__