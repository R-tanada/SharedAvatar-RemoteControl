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

#ifndef __GSTUTILS__
#define __GSTUTILS__

#include <gst/audio/audio.h>
#include <gst/gst.h>
#include <gst/video/video.h>

#include "../Resources/AudioFrame.h"
#include "flow/media/helpers/PixelFormat.h"

namespace avatarflow {
namespace media {

class AvaFlow_API GstUtils {
public:
  static GstVideoInfo getVideoInfo(GstSample *sample);
  static PixelFormat ConvertPixelFormat(GstVideoFormat src);
  static GstVideoFormat ConvertPixelFormat(PixelFormat src);
  static const char *GetGstVideoFormatStr(PixelFormat fmt);

  static AudioFormat GetGstAudioFormat(GstAudioFormat f, bool &_bigEndian);
  static GstAudioInfo getGstAudioInfo(GstSample *sample);

  static const char *GetGstAudioFormatStr(AudioFormat fmt, bool bigEndian);

  static bool IsElementAvailable(const std::string &name);
};

} // namespace media
} // namespace avatarflow

#endif //__GSTUTILS__