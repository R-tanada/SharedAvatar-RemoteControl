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

#ifndef __IAPPSINKHANDLER__
#define __IAPPSINKHANDLER__

#include <memory>

#include <string>
#include "utils/ListenerContainer.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AudioFrame;
class VideoFrame;

class IAppSinkHandler;

class IAppSinkHandlerListener {
public:
virtual ~IAppSinkHandlerListener(){}
  virtual void OnStreamPrepared(IAppSinkHandler *){};
  virtual void OnNewSampleArrived(IAppSinkHandler *){};
  virtual void OnSampleChanged(IAppSinkHandler *){}
};

class GstAppSinkHandlerImpl;

class AvaFlow_API IAppSinkHandler
    : public utils::ListenerContainer<IAppSinkHandlerListener *> {
public:
  DECLARE_FIRE_METHOD(OnStreamPrepared, (IAppSinkHandler * v), (v));
  DECLARE_FIRE_METHOD(OnNewSampleArrived, (IAppSinkHandler * v), (v));
  DECLARE_FIRE_METHOD(OnSampleChanged, (IAppSinkHandler * v), (v));

protected:
  GstAppSinkHandlerImpl *_impl;

public:
  IAppSinkHandler();
  virtual ~IAppSinkHandler();

  virtual void
  SetSink(void *s) = 0; // automatically links the sink with the call backs

  virtual void Close();

  virtual int process_sample(void *sample) = 0;
  static IAppSinkHandler *  CreateHandler(const std::string &type); //"video","audio"
};

} // namespace media
} // namespace avatarflow

#endif
