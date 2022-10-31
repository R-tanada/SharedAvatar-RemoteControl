

#ifndef ___ZipArchive___
#define ___ZipArchive___

#include "OS/IArchive.h"
#include <zzip/zzip.h>

namespace avatarOS {

class ZipArchive : public IArchive {
protected:
  struct sFileEntry {
    std::string fileName;
    std::string filePath;
    std::string dirPath;
    size_t compressSize;
    size_t uncompressSize;

    bool isfolder;
    IStream *fileData;

    bool operator<(const sFileEntry &o) const { return fileName < o.fileName; }
  };
  zzip_dir *m_zipDir;

  std::vector<sFileEntry> m_files;
  // GCPtr<WinFile> m_file;

  bool checkForError(int err, const std::string &desc);

public:
  ZipArchive(const std::string &name);
  virtual ~ZipArchive();

  virtual void UnloadArchive();

  virtual bool LoadArchive();

  virtual IStream *GetFile(const std::string &name, FILE_MODE mode);
  virtual IStream *GetFile(int index, FILE_MODE mode);

  virtual int GetFileIndex(const std::string &name);

  virtual void GetFilesList(bool dirs, TFiles_List &outList);

  virtual int GetCount(bool dir);
};

} // namespace avatarOS

#endif
