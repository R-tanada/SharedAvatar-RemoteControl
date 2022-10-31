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

#ifndef __ROBOTCONTROLMESSAGE__
#define __ROBOTCONTROLMESSAGE__

#include <utils/GUID.h>

#include <map>

#include "RobotMessages.h"
#include "flow/media/helpers/DataFrameParser.h"

namespace avatarflow {
namespace robot {

class RobotControlMessage {
public:
  enum EDataType { binary, text, json };

protected:
  RobotMessage _msg;
  EDataType _dataType;
  std::vector<utils::byte> _data;

  void _setData(const utils::uchar *d, utils::uint len) {
    _data.resize(len);
    for (int i = 0; i < len; ++i) {
      _data[i] = d[i];
    }
  }

public:
  RobotControlMessage(/* args */);
  ~RobotControlMessage();

  const RobotMessage &GetMessage() const { return _msg; }
  EDataType GetDataType() const { return _dataType; }
  const std::vector<utils::byte> &GetRawData() const { return _data; }
  std::map<std::string, std::string> GetJsonData() const;
  std::string GetStringData() const;

  void SetMessage(RobotMessage msg, const std::string &data);
  void SetMessage(
      RobotMessage msg,
      const std::map<std::string, std::string> &data); // set from a dictionary
  void SetMessage(RobotMessage msg, const utils::byte *data, int len);

  void Write(media::DataFrameParser &frame);
  bool Parse(media::DataFrameParser &frame);
};

} // namespace robot
} // namespace avatarflow

#endif //__ROBOTCONTROLMESSAGE__