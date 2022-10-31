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

#ifndef __AppVideoGrabber__
#define __AppVideoGrabber__

#include "IVideoGrabber.h"
#include "OS/FPSCalc.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

/**
 * @brief Helper class to create an application specific video grabber, this can
 * be used with app video device to hook into media pipelines
 *
 */
class AvaFlow_API AppVideoGrabber : public IVideoGrabber {
private:
  VideoFrame _frame;
  utils::ulong _lastFrame;
  utils::byte *_pixelsPtr;
  bool _newframe;
  avatarOS::FPSCalc _fpsCalc;

public:
  AppVideoGrabber();
  ~AppVideoGrabber();

  // Set the new frame value here
  void UpdateFrame(const VideoFrame *frame);

  virtual const utils::vector2di &GetFrameSize() { return _frame.GetSize(); }
  virtual PixelFormat GetImageFormat() { return _frame.GetFormat(); }

  virtual int GetFramesCount() { return 1; }
  virtual bool GrabFrame();
  virtual bool HasNewFrame();
  virtual utils::ulong GetBufferID(); // incremented once per frame
  virtual float GetCaptureFrameRate();

  virtual const VideoFrame *GetLastFrame() const;

  virtual void Lock();
  virtual void Unlock();
};

} // namespace media
} // namespace avatarflow

#endif //__AppVideoGrabber__