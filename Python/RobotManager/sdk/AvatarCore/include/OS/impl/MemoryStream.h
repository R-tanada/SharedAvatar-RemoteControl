

#ifndef ___MemoryStream___
#define ___MemoryStream___

#include "OS/IStream.h"

namespace avatarOS {

class MemoryStream : public IStream {
protected:
  utils::byte *data;
  int pos;
  bool bDeleteAtEnd;
  virtual utils::uint calcSizeInternal();

public:
  MemoryStream();
  MemoryStream(const std::string &name, utils::byte *data, int size,
               bool deleteAtEnd, FILE_MODE _mode = BIN_READ);
  virtual ~MemoryStream();

  virtual bool ReOpen(FILE_MODE mode);

  virtual bool CanRead() const;
  virtual bool CanWrite() const;
  bool CanSeek() const;

  void SetDeleteAtEnd(bool set);

  bool IsDeleteAtEnd();

  void SetData(utils::byte *data, int size);
  utils::byte *GetData();

  bool Open(const std::string &name, utils::byte *data, int size,
            FILE_MODE _mode);
  virtual bool Close();
  virtual bool IsOpen() const;

  virtual int Read(void *_data, size_t _size);
  virtual int Write(const void *_data, size_t _size);
  virtual int Seek(int bytes, EStreamSeek smode);
  virtual int GetPos() const;
  virtual bool EoF() const;
};

} // namespace avatarOS

#endif
