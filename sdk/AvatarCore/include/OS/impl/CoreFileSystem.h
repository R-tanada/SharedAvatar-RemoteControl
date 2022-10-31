#ifndef ___CoreFileSystem___
#define ___CoreFileSystem___

#include "OS/IFileSystem.h"
#include "OS/IStream.h"
#include <string>
#include <vector>

namespace avatarOS {

class CMemoryStream;

//! has common methods to work with files and directories
class CoreFileSystem : public IFileSystem {
  std::vector<std::string> dirsPaths;
  std::string appPath;
  std::string appName;

public:
  CoreFileSystem();
  virtual ~CoreFileSystem();

  IStream *OpenFile(const std::string &filename, FILE_MODE mode = BIN_READ);

  bool CreateDirectory(const std::string &dirName);

  void GetExecutableFullLocation(std::string &loc);

  bool FileExist(const std::string &fileName);

  std::string GetCurrPath();
  int GetCurrDrive();
  std::string GetCurrDriveName();

  std::string GetShortFileName(const std::string &name);

  virtual bool GetUserHomeDirectory(std::string &dir);
  virtual bool GetTempDirectory(std::string &dir);

  //! copy file from src-->dst
  void CopyFile(const std::string &src, const std::string &dst,
                bool overWrite = 0);

  std::string GetDriveVolume(char drv);
  void GetAllDrivesLetters(std::vector<char> &out);

  const std::string &GetAppPath();
  const std::string &GetAppName();
  void AddPath(const std::string &path);
  void RemovePath(const std::string &path);
  const std::vector<std::string> &GetPathes();
  void GetCorrectFilePath(const std::string &name, std::string &fileName);

  bool CreateDirs(const std::string &dirPath);

  IStream *CreateBinaryFileReader(const std::string &fname);
  IStream *CreateBinaryFileWriter(const std::string &fname);
  IStream *CreateTextFileReader(const std::string &fname);
  IStream *CreateTextFileWriter(const std::string &fname);

  IStream *CreateBinaryBufferReader(const std::string &name, utils::byte *data,
                                    int size, bool releaseAtEnd);
  IStream *CreateBinaryBufferWriter(const std::string &fname, utils::byte *data,
                                    int size, bool releaseAtEnd);
  IStream *CreateTextBufferReader(const std::string &fname, utils::byte *data,
                                  int size, bool releaseAtEnd);
  IStream *CreateTextBufferWriter(const std::string &fname, utils::byte *data,
                                  int size, bool releaseAtEnd);
  IStream *CreateMemoryBufferStream(IStream *stream);

  bool DeleteFile(const std::string &path);
};

}; // namespace avatarOS

#endif
