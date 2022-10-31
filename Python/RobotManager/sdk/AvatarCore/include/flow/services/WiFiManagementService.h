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

#ifndef __WiFiManagementService__
#define __WiFiManagementService__

#include "IService.h"

namespace avatarflow {
namespace services {

enum WiFiOperationMode {
  Auto,
  AdHoc,
  Managed,
  Master,
  Repeater,
  Secondary,
  Monitor,
  Unkown
};

enum WiFiEncryptionType { None, WEP, WPA, WPA2, Enterprise };

struct WiFiAPInfo {
public:
  WiFiAPInfo()
      : ESSID_index(-1), networkID(0), encryption(None), opMod(Unkown),
        frequency(-1), channel(-1), bitrate(0), TxPower(0), SignalLevel(0),
        NoiseLevel(-1), Quality(0) {}
  std::string ESSID;
  int ESSID_index;

  std::string nickname;
  std::string protocol;
  std::string dbusPath;

  WiFiEncryptionType encryption;
  std::string security;
  int networkID;

  WiFiOperationMode opMod;

  double frequency;
  int channel;

  std::string APAddress;
  std::string BSSID;

  int bitrate;

  int TxPower;

  int SignalLevel;
  double NoiseLevel;
  double Quality;

  void *_apData;
};

struct NetworkInterface {
  std::string name;
  std::string driver;
  int speed;

  void *_devicePtr;
};

class WiFiManagementServiceImpl;
class AvaFlow_API WiFiManagementService : public IService {
public:
  DECLARE_RTTI

  static const utils::GUID PATH_WIFIRequestScan;
  static const utils::GUID PATH_WIFIConnectAP;
  static const utils::GUID PATH_WIFIDisconnectAP;
  static const utils::GUID PATH_WIFIGetAPDetails;
  static const utils::GUID PATH_WIFIGetActiveSSID;
  static const utils::GUID PATH_WIFIForgetSSID;
  static const utils::GUID PATH_WIFISetEnable;

  static const utils::GUID PATH_WIFIRequestScanResult;
  static const utils::GUID PATH_WIFIGetAPDetailsResult;
  static const utils::GUID PATH_WIFIGetActiveSSIDResult;
  static const utils::GUID PATH_WIFIGetEnabled;

  static const utils::GUID PATH_WIFIOnDisconnected;
  static const utils::GUID PATH_WIFIOnEnableStatus;
  static const utils::GUID PATH_WIFIOnConnected;
  static const utils::GUID PATH_WIFIOnConnectFailed;
  static const utils::GUID PATH_WIFIOnWiFiSignal;

private:
  WiFiManagementServiceImpl *_impl;

public:
  WiFiManagementService();
  virtual ~WiFiManagementService();

  virtual bool StartService();
  virtual bool StopService();
  virtual bool IsRunning();

  virtual void Subscriber_OnMessage(const utils::GUID &serviceName,
                                    const utils::GUID &path,
                                    const std::string &data,
                                    utils::uint address, bool forwarded);
};
MAKE_SERVICE_FACTORY(WiFiManagementService, "WiFiManagement")

} // namespace services

} // namespace avatarflow

#endif //__WiFiManagementService__

#endif
#endif
