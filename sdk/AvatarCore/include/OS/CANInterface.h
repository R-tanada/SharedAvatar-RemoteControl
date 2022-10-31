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

#ifndef __CANINTERFACE__
#define __CANINTERFACE__

#include "utils/OSType.h"
#include <stdint.h>
#ifdef __LINUX__
#endif
#include <string>

namespace avatarOS {

class CANInterfaceImpl;
class PeripheralManager;

class CANInterface {

public:
  struct CANFrame {
    uint32_t id;
    uint8_t length;
#ifdef __LINUX__
    uint8_t data[8] __attribute__((aligned(8)));
#else
    uint8_t data[8];
#endif
  };

protected:
  CANInterfaceImpl *_impl;

  CANInterface();
  bool _OpenInterface(const std::string &iface);

  friend class PeripheralManager;

public:
  ~CANInterface();

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
   * @brief will perform a send action to CAN Bus
   *
   * @param frame
   * @return true
   * @return false
   */
  virtual bool Send(const CANFrame &frame);
  /**
   * @brief will perform a read action from CAN Bus
   *
   * @param frame
   * @return true
   * @return false
   */
  virtual bool Receive(CANFrame &frame);
  /**
   * @brief Will do send and receive one after the other, the same frame
   * parameter will be reused for sending and receiving. Use this method to
   * ensure a blocking action and to avoid thread racing for reading the CAN Bus
   *
   * @param frame
   * @return true
   * @return false
   */
  virtual bool SendReceive(CANFrame &frame);
};

} // namespace avatarOS

#endif //__CANINTERFACE__