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

#include <utils/OSType.h>

#ifndef __MOBILE__
#ifdef __unix__

#ifndef __LTEManagementService__
#define __LTEManagementService__

#include "IService.h"

namespace avatarflow {
namespace services {

class LTEManagementServiceImpl;
class AvaFlow_API LTEManagementService : public IService {
public:
  DECLARE_RTTI

  static const utils::GUID PATH_LTESetEnable;
  static const utils::GUID PATH_LTEGetEnabled;
  static const utils::GUID PATH_LTEGetInfo;
  static const utils::GUID PATH_LTEGetInfoReply;

  static const utils::GUID PATH_LTEOnEnableStatus;
  static const utils::GUID PATH_LTEOnLTESignal;

private:
  LTEManagementServiceImpl *_impl;

public:
  LTEManagementService();
  virtual ~LTEManagementService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();
  virtual void Update(float dt);

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
};
MAKE_SERVICE_FACTORY(LTEManagementService, "LTEManagement")

} // namespace services

} // namespace avatarflow

#endif //__LTEManagementService__

#endif
#endif
