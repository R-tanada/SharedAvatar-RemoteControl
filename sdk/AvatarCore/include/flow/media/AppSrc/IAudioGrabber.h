

/********************************************************************
        created:	2012/07/27
        created:	27:7:2012   13:54
        filename:
d:\Development\mrayEngine\Engine\mrayEngine\include\IAudioGrabber.h file path:
d:\Development\mrayEngine\Engine\mrayEngine\include file base:	IAudioGrabber
        file ext:	h
        author:		MHD YAMEN SARAIJI

        purpose:
*********************************************************************/
#ifndef ___IAudioGrabber___
#define ___IAudioGrabber___

#include "flow/media/Resources/AudioFrame.h"

namespace avatarflow {
namespace media {

class IAudioGrabber {
protected:
public:
  IAudioGrabber() {}
  virtual ~IAudioGrabber() {}

  virtual AudioFormat GetAudioFormat() = 0;
  virtual bool IsAudioBigEndian() = 0;
  virtual utils::uint GetSamplingRate() = 0;
  virtual utils::uint GetSamplesLength() = 0;
  virtual utils::uint GetChannelsCount() = 0;

  virtual bool GrabNoCopy(AudioFrame *f) = 0;
  virtual bool GrabFrame() = 0;
  virtual bool HasNewFrame() = 0;
  virtual utils::ulong GetBufferID() = 0; // incremented once per frame

  virtual const AudioFrame *GetLastFrame() const = 0;

  virtual void Reset() {}

  virtual int GetRemainingBufferLength() { return 0; }
  virtual int GetRemainingBufferDuration() { return 0; }
  virtual void Lock() {}
  virtual void Unlock() {}
};

} // namespace media
} // namespace avatarflow

#endif
