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

#ifndef __AUDIOAPPSINKHANDLER__
#define __AUDIOAPPSINKHANDLER__

#include "../AppSrc/IAudioGrabber.h"
#include "IAppSinkHandler.h"

namespace avatarflow {
namespace media {
class AudioFrame;

class AudioAppSinkHandlerImpl;
/**
 * @brief This class helps to hook into an app sink element to grap raw audio
 * packets from the media pipeline. It will trigger OnNewSampleArrived()
 * whenever new samples were processed. You can use it to process audio samples
 * in realtime without buffering or sampling (trigger based processing)
 *
 */
class AvaFlow_API AudioAppSinkHandler : public IAudioGrabber,
                                        public IAppSinkHandler {
protected:
  AudioAppSinkHandlerImpl *_audioImpl;

public:
  AudioAppSinkHandler();
  virtual ~AudioAppSinkHandler();

  virtual AudioFormat GetAudioFormat();
  virtual utils::uint GetSamplingRate();
  virtual utils::uint GetSamplesLength();
  virtual utils::uint GetChannelsCount();
  virtual bool IsAudioBigEndian();

  virtual bool GrabNoCopy(AudioFrame *f) { return false; }

  /**
   * @brief Copy audio samples, make sure output has enough size
   *
   * @param output
   */
  void CopyAudioFrame(utils::byte *output, utils::uint len);

  /**
   * @brief When this function is called, it will invalidate the state of the
   * sink, and will return whether it has a new buffer or not. Should be called
   * before fetching the buffer object
   *
   * @return true
   * @return false
   */
  virtual bool GrabFrame();

  /**
   * @brief Check if there is a new buffer without invalidating the statet of
   * the sink.
   *
   * @return true
   * @return false
   */
  virtual bool HasNewFrame();

  /**
   * @brief returns a unique ID of the sample (sequential)
   *
   * @return utils::ulong
   */
  virtual utils::ulong GetBufferID();

  virtual void SetSink(void *s);
  virtual void Close();

  /**
   * @brief Internally used by the app sink objec to process media samples
   *
   * @param sample
   * @return int
   */
  virtual int process_sample(void *sample);

  virtual const AudioFrame *GetLastFrame() const;
};

} // namespace media
} // namespace avatarflow

#endif