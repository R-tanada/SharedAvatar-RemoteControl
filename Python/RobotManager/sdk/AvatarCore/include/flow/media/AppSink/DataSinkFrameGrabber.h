

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

#ifndef __DataSINKFRAMEGRABBER__
#define __DataSINKFRAMEGRABBER__

#include "flow/avatarflow_config.h"
#include <utils/ListenerContainer.h>

namespace avatarflow {
namespace media {

class DataSinkFrameGrabber;
class DataAppSinkHandler;
class IDataAppSink;
class DataFrame;
class DataSinkFrameGrabberImpl;

class IDataSinkFrameGrabberListener {
public:
  virtual ~IDataSinkFrameGrabberListener() {}
  virtual void DataSink_OnNewFrame(DataSinkFrameGrabber *g,
                                   const DataFrame *frame) {}
};

class AvaFlow_API DataSinkFrameGrabber
    : public utils::ListenerContainer<IDataSinkFrameGrabberListener *> {
public:
  DECLARE_FIRE_METHOD(DataSink_OnNewFrame,
                      (DataSinkFrameGrabber * g, const DataFrame *frame),
                      (g, frame));

private:
  DataSinkFrameGrabberImpl *_impl;

public:
  DataSinkFrameGrabber();
  ~DataSinkFrameGrabber();
  void AttachSink(DataAppSinkHandler *sink);

  bool Start(bool triggerBased);
  bool Stop();
};

} // namespace media

} // namespace avatarflow

#endif //__DataSINKFRAMEGRABBER__