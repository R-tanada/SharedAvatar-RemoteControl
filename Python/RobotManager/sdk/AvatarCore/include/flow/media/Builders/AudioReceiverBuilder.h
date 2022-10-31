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

#ifndef __AudioReceiverBuilder__
#define __AudioReceiverBuilder__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>
#include <utils/TypedProperty.h>
#include <utils/atypes.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AvaFlow_API AudioReceiverBuilder : public utils::PropertySet {
    DECLARE_RTTI
   private:
   public:
    enum EType {
        None,
        Opus,
        Speex
    };
    std::string _encoder;

    EType _type;
    utils::uint _samplerate;
    std::string _format;
    int _channels;

   public:
    DECLARE_PROPERTY_TYPE(Encoder, std::string, );
    DECLARE_PROPERTY_TYPE(SampleRate, utils::uint, );  // Capture width
    DECLARE_PROPERTY_TYPE(Format, std::string, );
    DECLARE_PROPERTY_TYPE(Channels, int, );  // Capture width

    DECLARE_SETGET_PROP(Encoder, std::string);
    DECLARE_SETGET_PROP(SampleRate, utils::uint);
    DECLARE_SETGET_PROP(Format, std::string);
    DECLARE_SETGET_PROP(Channels, int);

   public:
    AudioReceiverBuilder();
    ~AudioReceiverBuilder();

    std::string BuildPipeline(bool addRtp = false, int payload = -1,
                              const std::string &prefix = "");

    std::string GetEncodingName();

    EType GetEncoderType() { return _type; }
};

}  // namespace media
}  // namespace avatarflow

#endif  //__AudioReceiverBuilder__