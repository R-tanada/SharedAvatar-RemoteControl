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

#ifndef __STREAMWRITER__
#define __STREAMWRITER__

#include "avataros_config.h"
#include "utils/atypes.h"
#include <string>
#include <utils/ListenerContainer.h>

namespace avatarOS {

class AvaOS_API IStream;
class AvaOS_API StreamWriter;

class IStreamWriterListener {
public:
  virtual void OnWriteData(StreamWriter *writer, const void *data,
                           size_t count) {}
};

class AvaOS_API StreamWriter
    : public utils::ListenerContainer<IStreamWriterListener *> {
private:
protected:
  IStream *m_stream;
  DECLARE_FIRE_METHOD(OnWriteData,
                      (StreamWriter * writer, const void *data, size_t count),
                      (writer, data, count));

public:
  StreamWriter();
  StreamWriter(IStream *stream);
  virtual ~StreamWriter();

  void SetStream(IStream *stream);
  IStream *GetStream();

  int WriteByte(utils::uchar v);
  int Write(const void *data, size_t count);
  int WriteLine(const std::string &str);
  int WriteString(const std::string &str);
  int BinWriteInt(int v);
  int TextWriteInt(int v);
  int BinWriteFloat(float v);
  int TextWriteFloat(float v);
  int BinWriteString(const std::string &str);

  template <class T> int WriteValue(const T &v) {
    return Write((const void *)&v, sizeof(T));
  }

  template <class T> StreamWriter &operator<<(const T &v) {
    WriteValue(v);
    return *this;
  }
};

} // namespace avatarOS

#endif //___StreamWriter___
