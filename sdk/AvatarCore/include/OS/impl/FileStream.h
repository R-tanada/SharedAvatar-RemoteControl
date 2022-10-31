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

#ifndef __FILESTREAM__
#define __FILESTREAM__

#include "OS/IStream.h"
#include <fstream>

namespace avatarOS {
class FileStream : public IStream {
private:
  // HANDLE m_fp;
  FILE *m_fp;
  virtual utils::uint calcSizeInternal();

public:
  FileStream();
  FileStream(const std::string &name, FILE_MODE mode = BIN_READ);

  virtual ~FileStream();

  bool Open(const std::string &name, FILE_MODE _mode = BIN_READ);
  virtual bool ReOpen(FILE_MODE _mode);

  virtual bool CanRead() const;
  virtual bool CanWrite() const;
  virtual bool CanSeek() const { return true; }

  bool ReOpen();

  virtual bool IsOpen() const;
  //! reads a block of data
  virtual int Read(void *data, size_t size);
  //! writes a block of data
  virtual int Write(const void *data, size_t size);
  //! seek file for number of bytes
  //! \param mode:seek type ( SEEK_SET,SEEK_CUR,SEEK_END)
  virtual int Seek(int bytes, EStreamSeek mode);

  virtual bool Close();

  //! return the current file pos
  virtual int GetPos() const;

  //! have we reach End Of File
  virtual bool EoF() const;
};

} // namespace avatarOS

#endif //__FILESTREAM__