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

#ifndef __SERIALINTERFACE__
#define __SERIALINTERFACE__

#include <string>
#include <vector>

#include "IStream.h"

namespace avatarOS {

class SerialInterfaceImpl;
class PeripheralManager;
class SerialInterface : public IStream {
public:
protected:
  SerialInterfaceImpl *_impl;
  SerialInterface();
  bool _OpenInterface(const std::string &iface, uint32_t baudrate);

  friend class PeripheralManager;
  utils::uint calcSizeInternal() { return sizeof(SerialInterface); }

public:
  ~SerialInterface();

  virtual bool CanRead() const { return true; }
  virtual bool CanWrite() const { return true; }
  virtual bool CanSeek() const { return false; }

  virtual int GetPos() const { return 0; }
  virtual bool IsStream() const { return true; }

  virtual bool ReOpen(FILE_MODE mode) { return false; }
  /**
   * @brief Check if its open
   *
   * @return true
   * @return false
   */
  virtual bool IsOpen() const;

  /**
   * @brief Close the interface
   *
   */
  virtual bool Close();

  /**
   * @brief will perform a write action
   *
   * @param frame
   * @return true
   * @return false
   */
  virtual int Write(const void *data, size_t size);

  /**
   * @brief Read bytes on the port
   *
   * @param data
   * @param count number of bytes to read
   * @return true
   * @return false
   */
  virtual int Read(void *data, size_t size);

  virtual int ReceiveString(std::string &data, int maxCount,
                            const std::string &eol = "\n");

  /**
   * @brief Check available bytes
   *
   * @return int
   */
  int AvailableBytes();

  virtual void Flush();
  virtual void FlushInput();
  virtual void FlushOutput();

  virtual int Seek(int bytes, EStreamSeek mode) { return -1; }
  virtual bool EoF() const { return !IsOpen(); }
};

} // namespace avatarOS

#endif //__SERIALINTERFACE__