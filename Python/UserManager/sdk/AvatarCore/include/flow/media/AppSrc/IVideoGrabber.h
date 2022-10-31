

/********************************************************************
        created:	2012/07/27
        created:	27:7:2012   13:54
        filename:
d:\Development\mrayEngine\Engine\mrayEngine\include\IVideoGrabber.h file path:
d:\Development\mrayEngine\Engine\mrayEngine\include file base:	IVideoGrabber
        file ext:	h
        author:		MHD YAMEN SARAIJI

        purpose:
*********************************************************************/
#ifndef ___IVideoGrabber___
#define ___IVideoGrabber___

#include "flow/media/Resources/VideoFrame.h"

namespace avatarflow {
namespace media {

class IVideoGrabber {
protected:
public:
  IVideoGrabber() {}
  virtual ~IVideoGrabber() {}

  virtual const utils::vector2di &GetFrameSize() = 0;
  virtual PixelFormat GetImageFormat() = 0;

  virtual int GetFramesCount() { return 1; }
  virtual bool GrabFrame() = 0;
  virtual bool HasNewFrame() = 0;
  virtual utils::ulong GetBufferID() = 0; // incremented once per frame
  virtual float GetCaptureFrameRate() = 0;

  virtual const VideoFrame *GetLastFrame() const = 0;

  virtual void Lock() {}
  virtual void Unlock() {}
};

} // namespace media
} // namespace avatarflow

#endif
