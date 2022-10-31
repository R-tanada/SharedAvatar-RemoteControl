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

#ifndef __STREAMREADER__
#define __STREAMREADER__

#include "avataros_config.h"
#include "utils/atypes.h"
#include <string>
#include <utils/ListenerContainer.h>

namespace avatarOS {

class AvaOS_API IStream;
class AvaOS_API StreamReader;

class IStreamReaderListener {
public:
  virtual void OnReadData(StreamReader *reader, const void *data,
                          size_t count) {}
};
class AvaOS_API StreamReader
    : public utils::ListenerContainer<IStreamReaderListener *> {
private:
protected:
  IStream *m_stream;
  DECLARE_FIRE_METHOD(OnReadData,
                      (StreamReader * reader, const void *data, size_t count),
                      (this, data, count));

public:
  StreamReader();
  StreamReader(IStream *stream);
  virtual ~StreamReader();

  void SetStream(IStream *stream);
  IStream *GetStream();

  utils::uchar ReadByte();
  int Read(void *data, size_t count);
  std::string ReadLine(char comment = '#');
  bool BinReadBool();
  int BinReadInt();
  long BinReadLong();
  int TextReadInt();
  float BinReadFloat();
  float TextReadFloat();
  std::string ReadString();

  template <class T> int ReadValue(T &v) { return Read((void *)&v, sizeof(T)); }

  std::string BinReadString();

  template <class T> StreamReader &operator>>(T &v) {
    ReadValue(v);
    return *this;
  }
};

} // namespace avatarOS

#endif //___StreamReader___
