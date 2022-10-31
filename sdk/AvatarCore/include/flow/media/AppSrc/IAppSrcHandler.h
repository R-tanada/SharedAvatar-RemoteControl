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

#ifndef __IAPPSRCHANDLER__
#define __IAPPSRCHANDLER__

#include "flow/avatarflow_config.h"
#include "utils/ListenerContainer.h"
#include "utils/atypes.h"

namespace avatarflow {
namespace media {

class IAppSrcHandler;
class GstPipeline;
class AppSrcHandlerImpl;

class AvaFlow_API IAppSrcHandlerListener {
public:
  virtual void OnNewSampleRequested(IAppSrcHandler *){};
  virtual void onStartFeed(IAppSrcHandler *src) {}
  virtual void onStopFeed(IAppSrcHandler *src) {}
  virtual void onSeekData(IAppSrcHandler *src, int offset) {}
};

class IAppSrcHandler
    : public utils::ListenerContainer<IAppSrcHandlerListener *> {
private:
protected:
  friend class AppSrcHandlerImpl;

  DECLARE_FIRE_METHOD(OnNewSampleRequested, (IAppSrcHandler * a), (a))
  DECLARE_FIRE_METHOD(onStartFeed, (IAppSrcHandler * a), (a))
  DECLARE_FIRE_METHOD(onStopFeed, (IAppSrcHandler * a), (a))
  DECLARE_FIRE_METHOD(onSeekData, (IAppSrcHandler * a, int offset), (a, offset))

  std::string _name;

public:
  IAppSrcHandler(const std::string &name) : _name(name) {}
  ~IAppSrcHandler() {}

  const std::string &GetName() const { return _name; }
  void SetName(const std::string &name) { _name = name; }

  virtual void SetSrc(void *s, GstPipeline *pipeline, unsigned int rate,
                      bool pullMode = false) = 0;
  virtual void Stop() = 0;
  static IAppSrcHandler *
  CreateHandler(const std::string &type,
                const std::string &name); //"video","audio"
};
} // namespace media
} // namespace avatarflow

#endif //__IAPPSRCHANDLER__