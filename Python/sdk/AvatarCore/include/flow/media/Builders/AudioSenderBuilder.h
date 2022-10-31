
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

#ifndef __AudioSenderBuilder__
#define __AudioSenderBuilder__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>
#include <utils/TypedProperty.h>
#include <utils/atypes.h>
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {
class AvaFlow_API AudioSenderBuilder : public utils::PropertySet {
    DECLARE_RTTI
   private:
   public:
    enum EType {
        None,
        Opus,
        Speex
    };

    EType _type;

    std::string _encoder;
    utils::uint _bitrate;
    utils::uint _samplerate;

    std::string _precodingListener;
    std::string _postcodingListener;
    std::string _postrtpListener;

   public:
    DECLARE_PROPERTY_TYPE(Encoder, std::string, );     // H264/VP8
    DECLARE_PROPERTY_TYPE(Bitrate, utils::uint, );     // Kb/s
    DECLARE_PROPERTY_TYPE(SampleRate, utils::uint, );  // Capture width

    DECLARE_SETGET_PROP(Encoder, std::string);
    DECLARE_SETGET_PROP(Bitrate, utils::uint);
    DECLARE_SETGET_PROP(SampleRate, utils::uint);

   public:
    AudioSenderBuilder();
    ~AudioSenderBuilder();

    EType GetEncoderType() { return _type; }
    std::string BuildPipeline(bool addRtp = false, int payload = 96,
                              const std::string &prefix = "");
    std::string GetRTPPayloader();
};

}  // namespace media

}  // namespace avatarflow

#endif  //__AudioSenderBuilder__