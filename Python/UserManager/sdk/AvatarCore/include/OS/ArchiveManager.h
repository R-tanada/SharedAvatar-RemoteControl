
#ifndef ___ArchiveManager___
#define ___ArchiveManager___

#include "IStream.h"
#include <utils/ISingleton.h>

#include "IArchive.h"
#include "IArchiveLoader.h"

namespace avatarOS {

class AvaOS_API ArchiveManager : public utils::ISingleton<ArchiveManager> {
  typedef std::map<std::string, IArchive *> ArchiveList;
  typedef std::map<std::string, IArchiveLoader *> ArchiveLoaders;

  ArchiveList m_Archives;
  ArchiveLoaders m_ArchiveLoaders;

public:
  ArchiveManager();
  virtual ~ArchiveManager();

  IArchive *AddArchive(const std::string &filename);
  IArchive *GetArchive(const std::string &archive);
  void RemoveArchive(const std::string &filename);

  IStream *OpenFile(const std::string &filename, const std::string &archive,
                    FILE_MODE mode);
  IStream *SearchForFile(const std::string &filename, FILE_MODE mode);

  void AddArchiveLoader(IArchiveLoader *loader);
  void RemoveArchiveLoader(const std::string &type);
};

#define gArchiveManager avatarOS::ArchiveManager::getInstance()

} // namespace avatarOS

#endif
