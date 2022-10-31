
/********************************************************************
        created:	2009/01/28
        created:	28:1:2009   0:08
        filename:
i:\Programing\GameEngine\mrayEngine\mrayEngine\include\FileResourceManager.h
        file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
        file base:	FileResourceManager
        file ext:	h
        author:		Mohamad Yamen Saraiji

        purpose:
*********************************************************************/

#ifndef ___FileResourceManager___
#define ___FileResourceManager___

#include "IResourceManager.h"
#include "IStream.h"
#include "utils/ISingleton.h"

namespace avatarOS {

class AvaOS_API FileResourceManager
    : public IResourceManager,
      public utils::ISingleton<FileResourceManager> {
protected:
  virtual IResource *CreateInternal(const std::string &name);

  virtual IResource *LoadResourceFromFile(IStream *file);

  virtual std::string GetDefaultGroup();

public:
  FileResourceManager();
  virtual ~FileResourceManager();

  IStream *GetFile(const std::string &name, FILE_MODE mode = BIN_READ);

  virtual void WriteResourceToDist(const std::string &resName,
                                   const std::string &fileName);
  virtual void WriteResourceToDist(const IResource *&resource,
                                   const std::string &fileName);
  virtual void WriteResourceToDist(ResourceHandle resource,
                                   const std::string &fileName);
};
#define gFileResourceManager avatarOS::FileResourceManager::getInstance()

} // namespace avatarOS

#endif //___FileResourceManager___
