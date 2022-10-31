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

#ifndef __IFILESYSTEM__
#define __IFILESYSTEM__

#include "IStream.h"
#include <utils/ILogManager.h>
#include <utils/ISingleton.h>

namespace avatarOS {

//! has common methods to work with files and directories
class AvaOS_API IFileSystem : public utils::ISingleton<IFileSystem> {

public:
  IFileSystem() {}
  virtual ~IFileSystem() {}

  virtual IStream *OpenFile(const std::string &filename,
                            FILE_MODE mode = BIN_READ) = 0;

  virtual bool CreateDirectory(const std::string &dirName) = 0;

  virtual void GetExecutableFullLocation(std::string &loc) = 0;

  virtual bool FileExist(const std::string &fileName) = 0;

  virtual std::string GetCurrPath() = 0;
  virtual int GetCurrDrive() = 0;
  virtual std::string GetCurrDriveName() = 0;

  virtual std::string GetShortFileName(const std::string &name) = 0;

  virtual bool GetUserHomeDirectory(std::string &dir) = 0;
  virtual bool GetTempDirectory(std::string &dir) = 0;

  //! copy file from src-->dst
  virtual void CopyFile(const std::string &src, const std::string &dst,
                        bool overWrite = 0) = 0;

  virtual void GetAllDrivesLetters(std::vector<char> &out) = 0;
  virtual std::string GetDriveVolume(char drv) = 0;

  virtual const std::string &GetAppPath() = 0;
  virtual const std::string &GetAppName() = 0;
  virtual void AddPath(const std::string &path) = 0;
  virtual void RemovePath(const std::string &path) = 0;
  virtual const std::vector<std::string> &GetPathes() = 0;
  virtual void GetCorrectFilePath(const std::string &name,
                                  std::string &fileName) = 0;

  virtual bool CreateDirs(const std::string &dirPath) = 0;

  virtual IStream *CreateBinaryFileReader(const std::string &fname) = 0;
  virtual IStream *CreateBinaryFileWriter(const std::string &fname) = 0;
  virtual IStream *CreateTextFileReader(const std::string &fname) = 0;
  virtual IStream *CreateTextFileWriter(const std::string &fname) = 0;

  virtual IStream *CreateBinaryBufferReader(const std::string &name,
                                            utils::byte *data, int size,
                                            bool releaseAtEnd) = 0;
  virtual IStream *CreateBinaryBufferWriter(const std::string &fname,
                                            utils::byte *data, int size,
                                            bool releaseAtEnd) = 0;
  virtual IStream *CreateTextBufferReader(const std::string &fname,
                                          utils::byte *data, int size,
                                          bool releaseAtEnd) = 0;
  virtual IStream *CreateTextBufferWriter(const std::string &fname,
                                          utils::byte *data, int size,
                                          bool releaseAtEnd) = 0;
  virtual IStream *CreateMemoryBufferStream(IStream *stream) = 0;

  virtual bool DeleteFile(const std::string &path) = 0;
};

#define gFileSystem avatarOS::IFileSystem::getInstance()

} // namespace avatarOS

#endif //___IFileSystem___
