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

#ifndef __STREAMWRITEREX__
#define __STREAMWRITEREX__

#include "StreamWriter.h"
#include <string>

#include <utils/box3D.h>
#include <utils/line2D.h>
#include <utils/line3D.h>
#include <utils/matrix4x4.h>
#include <utils/point2D.h>
#include <utils/point3D.h>
#include <utils/point4D.h>
#include <utils/quaternion.h>

namespace avatarOS {

class AvaOS_API StreamWriterEx {
protected:
  StreamWriter *m_writer;

public:
  StreamWriterEx(StreamWriter *wrtr) : m_writer(wrtr) {}
  virtual ~StreamWriterEx() {}

  StreamWriterEx &operator<<(int v);
  StreamWriterEx &operator<<(float v);
  StreamWriterEx &operator<<(const std::string &v);

  template <class T> StreamWriterEx &operator<<(const T &v) {
    m_writer->WriteValue(v);
    return *this;
  }

  StreamWriterEx &operator<<(const utils::vector2d &v);
  StreamWriterEx &operator<<(const utils::vector3d &v);
  StreamWriterEx &operator<<(const utils::vector4d &v);
  StreamWriterEx &operator<<(const utils::quaternion &v);
  StreamWriterEx &operator<<(const utils::matrix4x4 &v);
  StreamWriterEx &operator<<(const utils::line2d &v);
  StreamWriterEx &operator<<(const utils::line3d &v);
  StreamWriterEx &operator<<(const utils::box3d &v);
};

} // namespace avatarOS

#endif
