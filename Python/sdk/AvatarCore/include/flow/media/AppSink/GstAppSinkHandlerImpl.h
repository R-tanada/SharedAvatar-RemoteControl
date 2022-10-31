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

#ifndef __GstAppSinkHandlerImpl__
#define __GstAppSinkHandlerImpl__

#include <gst/app/gstappsink.h>
#include <gst/gst.h>

#include "flow/avatarflow_config.h"
#include <memory>

namespace avatarflow {
namespace media {
class IAppSinkHandler;

class AvaFlow_API GstAppSinkHandlerImpl {
private:
  GstAppSink *_sink;
  IAppSinkHandler *_owner;
  bool _closed;

public:
  GstAppSinkHandlerImpl(IAppSinkHandler *owner);
  ~GstAppSinkHandlerImpl();

  virtual void
  SetSink(void *s); // automatically links the sink with the call backs

  virtual void Close();

  virtual GstFlowReturn preroll_cb(std::shared_ptr<GstSample> buffer);
  virtual GstFlowReturn buffer_cb(std::shared_ptr<GstSample> buffer);
  virtual void eos_cb();

  static GstFlowReturn on_new_buffer_from_source(GstAppSink *elt, void *data);
  static GstFlowReturn on_new_preroll_from_source(GstAppSink *elt, void *data);
  static gboolean on_new_event_from_source(GstAppSink *elt, void *data);
  static void on_eos_from_source(GstAppSink *elt, void *data);
};

} // namespace media
} // namespace avatarflow

#endif //__GstAppSinkHandlerImpl__