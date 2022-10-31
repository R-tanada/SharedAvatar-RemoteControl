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

#ifndef __DataRECEIVERSERVICE__
#define __DataRECEIVERSERVICE__

#include "IService.h"
#include "utils/IDelegate.h"

namespace avatarflow {
namespace media {
class GstPipeline;
class IDataGrabber;
class DataFrame;
class DataSinkFrameGrabber;
} // namespace media
namespace services {

class DataReceiverServiceImpl;
class AvaFlow_API DataReceiverService : public IService {
public:
  DECLARE_RTTI

  typedef utils::IDelegate1<void, media::DataFrame *> *OnDataFrameDelegate;
  OnDataFrameDelegate OnDataFrame;

protected:
  DataReceiverServiceImpl *_impl;

public:
  DataReceiverService(/* args */);
  virtual ~DataReceiverService();

  virtual bool StartService();
  virtual bool StopService();

  media::GstPipeline *GetGstPipeline();
  virtual bool IsRunning();

  media::IDataGrabber *GetDataGrabber();
  media::DataSinkFrameGrabber *GetSinkGrabber();

  virtual void LoadSettings(const JSONObject &settings);
};
MAKE_SERVICE_FACTORY(DataReceiverService, "DataReceiver")

} // namespace services
} // namespace avatarflow

#endif //__DataRECEIVERSERVICE__