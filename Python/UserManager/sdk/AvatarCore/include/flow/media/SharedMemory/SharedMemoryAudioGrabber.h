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

#ifndef __SHAREDMEMORYAudioGRABBER__
#define __SHAREDMEMORYAudioGRABBER__
#include <utils/SharedMemory.h>

#include "../AppSrc/IAudioGrabber.h"
#include "../Resources/AudioResourceHeader.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class ISharedMemoryAudioGrabberListener {
public:
  virtual ~ISharedMemoryAudioGrabberListener() {}
};

class AvaFlow_API SharedMemoryAudioGrabber : public IAudioGrabber {
private:
  AudioResourceHeader *_header;
  AudioFrame _frame;
  utils::ulong _lastFrame;
  utils::SharedMemory *_sharedMemroy;
  utils::uint _currentTop;

  utils::byte *_samplesPtr;
  std::string _Resource;

  bool _alwaysAvailbleMode;

public:
  SharedMemoryAudioGrabber();
  ~SharedMemoryAudioGrabber();

  bool Open(const std::string &shmem);
  void Close();
  const std::string &GetName() const { return _Resource; }

  void SetAlwaysAvailble(bool e) { _alwaysAvailbleMode = e; }

  bool IsOpen() { return _header != 0; }

  virtual AudioFormat GetAudioFormat();
  virtual bool IsAudioBigEndian();
  virtual utils::uint GetSamplingRate();
  virtual utils::uint GetSamplesLength();
  virtual utils::uint GetChannelsCount();

  virtual bool GrabFrame();
  virtual bool GrabNoCopy(AudioFrame *f);

  virtual bool HasNewFrame();
  virtual utils::ulong GetBufferID(); // incremented once per frame

  virtual const AudioFrame *GetLastFrame() const;

  virtual void Reset();
  virtual void Lock();
  virtual void Unlock();

  virtual int GetRemainingBufferLength();
  virtual int GetRemainingBufferDuration();

  CoreResourceProvider GetResourceProvider();
};

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYAudioGRABBER__