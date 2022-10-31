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

#ifndef __ShmemVideoAppSinkHandler__
#define __ShmemVideoAppSinkHandler__

#include "../AppSrc/IVideoGrabber.h"
#include "../Resources/CoreResource.h"
#include "../Resources/VideoFrame.h"
#include "IAppSinkHandler.h"
#include "OS/FPSCalc.h"

namespace avatarflow {
namespace media {

class ShmemVideoAppSinkHandlerImpl;
class AvaFlow_API ShmemVideoAppSinkHandler : public IVideoGrabber,
                                             public IAppSinkHandler {
public:
protected:
  ShmemVideoAppSinkHandlerImpl *_videoImpl;

public:
  ShmemVideoAppSinkHandler();
  virtual ~ShmemVideoAppSinkHandler();

  /**
   * @brief Set the name of this shared memory and its size to allocate in bytes
   *
   * @param name
   * @param size
   */
  void SetName(const std::string &name, utils::uint size,
               CoreResourceProvider provider = Provider_Unkown);

  /**
   * @brief Set the GStreamer Sink object to listen to its data
   *
   * @param s
   */
  virtual void SetSink(void *s);

  virtual const utils::vector2di &GetFrameSize();
  virtual PixelFormat GetImageFormat();

  virtual void Close();
  virtual bool HasNewFrame();
  virtual int GetFramesCount();
  virtual bool GrabFrame();
  virtual utils::ulong GetBufferID();
  float GetCaptureFrameRate();

  /**
   * @brief Get the Last video frame available from this sink
   *
   * @return const VideoFrame*
   */
  const VideoFrame *GetLastFrame() const;

  /**
   * @brief Called internally to process an image frame from gstreamer
   *
   * @param sample
   * @return int
   */
  virtual int process_sample(void *sample);
};

} // namespace media

} // namespace avatarflow

#endif //__ShmemVideoAppSinkHandler__