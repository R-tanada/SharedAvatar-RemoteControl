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

#ifndef __VideoSenderBuilder__
#define __VideoSenderBuilder__

#include <utils/PropertySet.h>
#include <utils/StringUtil.h>
#include <utils/TypedProperty.h>
#include <utils/atypes.h>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class GstPipeline;
class IVideoEncoderController {
   public:
    virtual ~IVideoEncoderController() {}
    virtual void SetQuality(float qual) = 0;
    virtual float GetQuality() = 0;

    virtual void LinkPipeline(GstPipeline *pipeline) = 0;
};
class AvaFlow_API VideoSenderBuilder : public utils::PropertySet {
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
    utils::uint _bitrate;
    utils::uint _quality;
    utils::uint _fpsLimit;
    float _scaler;

    std::string _precodingListener;
    std::string _postcodingListener;
    std::string _postrtpListener;

    EType _type;

    std::map<std::string, std::string> _namekeys;

    IVideoEncoderController *_controller;

   public:
    DECLARE_PROPERTY_TYPE(Encoder, std::string, );   // H264/VP8
    DECLARE_PROPERTY_TYPE(Bitrate, utils::uint, );   // Kb/s
    DECLARE_PROPERTY_TYPE(Quality, utils::uint, );   // 1(low)~10(best)
    DECLARE_PROPERTY_TYPE(FPSLimit, utils::uint, );  // Framerate limiter
    DECLARE_PROPERTY_TYPE(VideoScale, float, );      // Framerate limiter

    DECLARE_SETGET_PROP(Encoder, std::string);
    DECLARE_SETGET_PROP(Bitrate, utils::uint);
    DECLARE_SETGET_PROP(Quality, utils::uint);
    DECLARE_SETGET_PROP(FPSLimit, utils::uint);
    DECLARE_SETGET_PROP(VideoScale, float);

   public:
    VideoSenderBuilder();
    ~VideoSenderBuilder();

    void GetEncoderParams(std::map<std::string, std::string> &nameKeys);
    std::string GetEncoderParam(const std::string &key);

    EType GetEncoderType() { return _type; }
    std::string BuildPipeline(bool addRtp = false, int payload = -1,
                              const std::string &prefix = "");
    std::string GetRTPPayloader();

    virtual void LinkWithPipeline(GstPipeline *pipeline);

    IVideoEncoderController *GetController() { return _controller; }
};

}  // namespace media

}  // namespace avatarflow

#endif  //__VideoSenderBuilder__