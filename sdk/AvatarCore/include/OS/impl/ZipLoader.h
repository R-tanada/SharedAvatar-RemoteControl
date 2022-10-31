

#ifndef ___ZipLoader___
#define ___ZipLoader___

#include "OS/IArchiveLoader.h"

namespace avatarOS {

class ZipLoader : public IArchiveLoader {
public:
  virtual ~ZipLoader();
  virtual IArchive *LoadArchive(const std::string &name);
  virtual std::string GetArchiveType();
  virtual bool CanLoadArchive(const std::string &name);
};

} // namespace avatarOS

#endif
