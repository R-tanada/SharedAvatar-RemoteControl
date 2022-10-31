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

#ifndef __GSTMYLISTENER__
#define __GSTMYLISTENER__

#include <flow/avatarflow_config.h>
#include <gst/gst.h>
#include <utils/ListenerContainer.h>

namespace avatarflow {
namespace media {

G_BEGIN_DECLS

/* #defines don't like whitespacey bits */
#define GST_TYPE_MyListener (gst_my_listener_get_type())
#define GST_MyListener(obj)                                                    \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_MyListener, GstMyListener))
#define GST_MyListener_CLASS(klass)                                            \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_MyListener, GstMyListenerClass))
#define GST_IS_MyListener(obj)                                                 \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_MyListener))
#define GST_IS_MyListener_CLASS(klass)                                         \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_MyListener))

typedef struct _GstMyListener GstMyListener;
typedef struct _GstMyListenerClass GstMyListenerClass;

class IMyListenerCallback {
public:
  virtual ~IMyListenerCallback() {}
  virtual void ListenerOnDataChained(_GstMyListener *src, GstBuffer *buffer) {}
};

class MyListenerContainer
    : public utils::ListenerContainer<IMyListenerCallback *> {
public:
  DECLARE_FIRE_METHOD(ListenerOnDataChained,
                      (_GstMyListener * src, GstBuffer *buffer), (src, buffer));
};
struct _GstMyListener {
  GstElement element;

  GstPad *sinkpad, *srcpad;
  GstCaps *caps;
  GstBuffer *buffer;
  bool _capsUpdated;

  gboolean silent;

  MyListenerContainer *listeners;
};

struct AvaFlow_API _GstMyListenerClass {
  GstElementClass parent_class;
  static gboolean plugin_init(GstPlugin *plugin);
};

GType AvaFlow_API gst_my_listener_get_type(void);

G_END_DECLS
} // namespace media
} // namespace avatarflow

#endif //__GSTMYLISTENER__