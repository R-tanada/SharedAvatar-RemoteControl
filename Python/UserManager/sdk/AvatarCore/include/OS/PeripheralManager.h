
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

#ifndef __PeripheralManager__
#define __PeripheralManager__

#include <utils/ISingleton.h>
#include <utils/atypes.h>

#include <list>
#include <map>
#include <vector>

#include "avataros_config.h"

namespace avatarOS {

class CANInterface;
class SerialInterface;

class AvaOS_API PeripheralManager
    : public utils::ISingleton<PeripheralManager> {
private:
  /* data */
public:
  PeripheralManager(/* args */);
  ~PeripheralManager();

  /**
   * @brief List available serial interfaces
   *
   * @return std::list<std::string>
   */
  virtual std::list<std::string> ListSerialPorts();

  /**
   * @brief Open a Serial Interface object, will return null on failure
   *
   * @param iface
   * @return SerialInterface*
   */
  SerialInterface *CreateSerialInterface(const std::string &iface,
                                         int baudrate);

  /*ˆ
   * @brief  Open a CAN Bus object, will return null on failure
   *
   * @param iface
   * @return CANInterface*
   */
  CANInterface *CreateCANAdapter(const std::string &iface);
};

#define gPeripheralManager avatarOS::PeripheralManager::getInstance()

} // namespace avatarOS

#endif //__PeripheralManager__