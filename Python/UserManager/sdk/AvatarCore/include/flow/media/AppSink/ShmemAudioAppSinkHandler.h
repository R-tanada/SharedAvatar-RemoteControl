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

#ifndef __ShmemAudioAppSinkHandler__
#define __ShmemAudioAppSinkHandler__

#include "../AppSrc/IAudioGrabber.h"
#include "../Resources/AudioFrame.h"
#include "../Resources/CoreResource.h"
#include "IAppSinkHandler.h"
#include "OS/FPSCalc.h"

namespace avatarflow {
namespace media {

class ShmemAudioAppSinkHandlerImpl;
class AvaFlow_API ShmemAudioAppSinkHandler : public IAudioGrabber,
                                             public IAppSinkHandler {
public:
protected:
  ShmemAudioAppSinkHandlerImpl *_AudioImpl;

public:
  ShmemAudioAppSinkHandler();
  virtual ~ShmemAudioAppSinkHandler();

  void SetName(const std::string &name, utils::uint size,
               CoreResourceProvider provider = Provider_Unkown);

  virtual void SetSink(void *s);

  virtual void Close();

  virtual AudioFormat GetAudioFormat();
  virtual bool IsAudioBigEndian();
  virtual utils::uint GetSamplingRate();
  virtual utils::uint GetSamplesLength();
  virtual utils::uint GetChannelsCount();

  virtual bool GrabNoCopy(AudioFrame *f) { return false; }
  virtual bool GrabFrame();
  virtual bool HasNewFrame();
  virtual utils::ulong GetBufferID(); // incremented once per frame

  virtual const AudioFrame *GetLastFrame() const;

  virtual int process_sample(void *sample);
};

} // namespace media

} // namespace avatarflow

#endif //__ShmemAudioAppSinkHandler__