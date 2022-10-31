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

#ifndef __APPSRCHANDLERIMPL__
#define __APPSRCHANDLERIMPL__

#include <gst/app/gstappsrc.h>
#include <gst/gst.h>
#include <utils/ListenerContainer.h>

#include <mutex>
#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class AppSrcHandlerImpl;
class IAppSrcHandler;
class GstPipeline;

class AvaFlow_API IAppSrcHandlerCallback {
public:
  virtual bool onNeedBuffer(AppSrcHandlerImpl *src, GstBuffer **buffer,
                            unsigned int &length) {
    return false;
  }
  virtual void onStartFeed(AppSrcHandlerImpl *src) {}
  virtual void onStopFeed(AppSrcHandlerImpl *src) {}
  virtual void onSeekData(AppSrcHandlerImpl *src, int offset) {}
};

class AppSrcHandlerImpl {
protected:
  GstAppSrcCallbacks srcCB;
  GstAppSrc *DataSrc;
  int sourceID;
  int _counter;
  int _sampleRate;
  bool _pullMode;
  GstPipeline *_pipeline;

  std::recursive_mutex _stateMutex;

  bool _isStopped;
  bool _suspendFeed;

  IAppSrcHandlerCallback *_listener;

  IAppSrcHandler *_owner;

  static gboolean read_data(AppSrcHandlerImpl *impl);
  static void start_feed(GstAppSrc *source, guint size, gpointer o);
  static void stop_feed(GstAppSrc *source, gpointer o);
  static gboolean seek_data(GstAppSrc *src, guint64 offset, gpointer o);

public:
  AppSrcHandlerImpl(IAppSrcHandler *owner);
  virtual ~AppSrcHandlerImpl();

  void SetSrc(void *s, IAppSrcHandlerCallback *listener, GstPipeline *pipeline,
              unsigned int sampleRate, bool pullMode = false);

  void Stop();

  const std::string &GetName() const;

  bool IsStarted() { return sourceID != 0; }
};

} // namespace media
} // namespace avatarflow

#endif //__APPSRCHANDLERIMPL__