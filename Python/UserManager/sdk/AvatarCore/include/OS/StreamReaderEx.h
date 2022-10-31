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

#ifndef __STREAMREADEREX__
#define __STREAMREADEREX__

#include "StreamReader.h"
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

class AvaOS_API StreamReaderEx {
protected:
  StreamReader *m_reader;

public:
  StreamReaderEx(StreamReader *rdr) : m_reader(rdr) {}
  virtual ~StreamReaderEx() {}

  StreamReaderEx &operator>>(bool &v);
  StreamReaderEx &operator>>(int &v);
  StreamReaderEx &operator>>(float &v);
  StreamReaderEx &operator>>(std::string &v);

  StreamReaderEx &operator>>(utils::vector2d &v);
  StreamReaderEx &operator>>(utils::vector3d &v);
  StreamReaderEx &operator>>(utils::vector4d &v);
  StreamReaderEx &operator>>(utils::quaternion &v);
  StreamReaderEx &operator>>(utils::matrix4x4 &v);
  StreamReaderEx &operator>>(utils::line2d &v);
  StreamReaderEx &operator>>(utils::line3d &v);
  StreamReaderEx &operator>>(utils::box3d &v);
};

} // namespace avatarOS

#endif
