

#ifndef ___IArchive___
#define ___IArchive___

#include "IStream.h"
#include <string>

namespace avatarOS {

class IArchive {
  std::string m_name;

public:
  typedef std::vector<std::string> TFiles_List;

public:
  IArchive(const std::string &name) : m_name(name) {}

  virtual ~IArchive() {}

  virtual void UnloadArchive() = 0;
  virtual bool LoadArchive() = 0;

  virtual IStream *GetFile(const std::string &name, FILE_MODE mode) = 0;
  virtual IStream *GetFile(int index, FILE_MODE mode) = 0;

  virtual int GetFileIndex(const std::string &name) = 0;

  virtual int GetCount(bool dir) = 0;

  virtual void GetFilesList(bool dirs, TFiles_List &outList) = 0;

  const std::string &GetName() { return m_name; }
};

} // namespace avatarOS

#endif
