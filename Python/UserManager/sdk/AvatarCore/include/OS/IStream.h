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

#ifndef __ISTREAM__
#define __ISTREAM__

#include "IResource.h"

namespace avatarOS {

enum EStreamSeek { ESeek_Set, ESeek_Cur, ESeek_End };

//! file open types
enum FILE_MODE {
  //! open binary file for reading
  BIN_READ = 0,
  //! open binary file for writting
  BIN_WRITE,
  //! open binary file for appending
  BIN_APPEND,
  //! open text file for reading
  TXT_READ,
  //! open text file for writting
  TXT_WRITE,
  //! open text file for appending
  TXT_APPEND
};

class AvaOS_API IStream : public IResource {
protected:
  std::string m_streamName;
  FILE_MODE m_mode;
  int m_length;

public:
  IStream();
  IStream(const std::string &name, FILE_MODE mode = BIN_READ);

  virtual ~IStream();

  void SetMode(FILE_MODE mode);

  virtual bool CanRead() const = 0;
  virtual bool CanWrite() const = 0;
  virtual bool CanSeek() const = 0;
  //! get size of file
  virtual int Size() const;
  virtual int GetPos() const = 0;
  // is it a continus stream (like network stream)
  virtual bool IsStream() const { return false; }

  const std::string &GetStreamName() const;

  virtual bool ReOpen(FILE_MODE mode) { return true; }

  virtual bool IsOpen() const = 0;
  virtual int Read(void *data, size_t size) = 0;
  virtual int Write(const void *data, size_t size) = 0;
  virtual int Seek(int bytes, EStreamSeek mode) = 0;
  virtual bool EoF() const = 0;
  virtual bool Close() = 0;
  int ReadToEnd(void **data);

  virtual int WriteByte(utils::uchar v);
  virtual utils::uchar ReadByte();
};

}; // namespace avatarOS

#endif
