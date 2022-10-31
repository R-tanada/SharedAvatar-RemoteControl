

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

#ifndef __AUDIOFRAME__
#define __AUDIOFRAME__

#include "flow/avatarflow_config.h"
#include "flow/media/helpers/AudioFormat.h"
#include "utils/atypes.h"

namespace avatarflow {
namespace media {

/**
 * @brief This class contains an array of samples representing an audio frame.
 * It contains all necessary information about its format, channels, and
 * sampling rate.
 * \n Also, it can be pointing to a shared memory, and thus it will manage not
 * to destroy when the object is deleted
 *
 * an audio frame represents an array of samples, each sample has a specific
 * size and contains adjacent channels, e.g. for stereo audio, the samples look
 * like [12][12][12]...., where 1 is the first channel, and 2 is the second
 * channel
 */
class AvaFlow_API AudioFrame {
public:
  /***** Internal Data, only use when its a must!! *****/

  int _channels;
  utils::byte *_samples;
  bool _autoDelete; // automatically delete the pixels when destroyed

  utils::uint _samplingRate;
  utils::uint _sampleslength;
  utils::uint _samplesSize;
  utils::uint _poolSize;
  AudioFormat _format;
  bool _bigEndian;

  bool _shmem;
  utils::ullong _timestamp;
  utils::ulong _shmemOffset;
  void _DeleteSafe(); // delete the samples safely

public:
  static int GetSampleSize(AudioFormat fmt);

public:
  AudioFrame(/* args */);
  ~AudioFrame();
  void SetSharedMemoryOffset(utils::ulong shmemOffset);
  void SetSharedMemoryPtr(utils::byte *ptr);

  /**
   * @brief Get the Sampling Rate of this audio frame
   *
   * @return utils::uint
   */
  utils::uint GetSamplingRate() const { return _samplingRate; }
  /**
   * @brief Get the Samples Length of this audio frame (not size!).
   *
   * @return utils::uint
   */
  utils::uint GetSamplesLength() const { return _sampleslength; }
  /**
   * @brief Get the Samples Size of this audio frame in bytes
   *
   * @return utils::uint
   */
  utils::uint GetSamplesSize() const { return _samplesSize; }
  /**
   * @brief Get the number of channels this audio frame has
   *
   * @return int
   */
  int GetChannelsCount() const { return _channels; }
  AudioFormat GetFormat() const { return _format; }
  bool IsBigEndian() const { return _bigEndian; }

  /**
   * @brief Get the samples data array of this frame, it manages whether its
   * from a shared memory or dedicated memory.
   *
   * @return utils::byte*
   */
  utils::byte *GetSamplesPtr();
  const utils::byte *GetSamplesPtr() const;

  /**
   * @brief whether or not the data is from a shared memory
   *
   * @return true
   * @return false
   */
  bool IsSharedMemoryPtr() const;

  /**
   * @brief whether to auto delete or not when done
   *
   * @return true
   * @return false
   */
  bool IsAutoDelete() const;

  /**
   * @brief Set the Time Stamp of this frame
   *
   * @param ts
   */
  void SetTimeStamp(utils::ullong ts);

  /**
   * @brief Get the Time Stamp
   *
   * @return utils::ullong
   */
  utils::ullong GetTimeStamp() const;

  /**
   * @brief Initialize the audio frame object, creating internal memory using
   * provided information about the requested audio frame specs
   *
   *
   * @param length number of samples for this audio frame (not bytes size)
   * @param samplingRate the sampling rate
   * @param channels number of channels per sample
   * @param fmt the format of audio samples
   * @param bigEndian is it big endian representation?
   * @param hardReset
   */
  void InitFrame(utils::uint length, utils::uint samplingRate, int channels,
                 AudioFormat fmt, bool bigEndian, bool hardReset = true);

  /**
   * @brief Initialize the audio frame object as a copy from existing audio
   * samples
   *
   *
   * @param length number of samples for this audio frame (not bytes size)
   * @param samplingRate the sampling rate
   * @param channels number of channels per sample
   * @param data bytes array of the data samples to copy from
   * @param fmt the format of audio samples
   * @param bigEndian is it big endian representation?
   * @param hardReset
   */
  void InitFrame(utils::uint length, utils::uint samplingRate, int channels,
                 utils::byte *data, AudioFormat fmt, bool bigEndian,
                 bool hardReset = true);

  /**
   * @brief Clone this audio frame to another frame
   *
   * @param to
   * @return AudioFrame*
   */
  AudioFrame *Clone(AudioFrame *to, utils::byte *shmemPtr = 0) const;
};

} // namespace media
} // namespace avatarflow

#endif //__AUDIOFRAME__