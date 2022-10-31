

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

#ifndef __VIDEOFRAME__
#define __VIDEOFRAME__

#include "flow/avatarflow_config.h"
#include "flow/media/helpers/PixelFormat.h"
#include "utils/atypes.h"
#include "utils/point2D.h"

namespace avatarflow {
namespace media {

/**
 * @brief This class contains an array of pixels representing an image. It
 * contains all necessary information about its format, row strides, and
 * offsets for the surfaces. It can be from almost any image format provided by
 * gstreamer. \n
 * Also, it can be pointing to a shared memory, and thus it will manage not to
 * destroy when the object is deleted
 *
 */
class AvaFlow_API VideoFrame {
public:
  /***** Internal Data, only use when its a must!! *****/

  utils::vector2di
      _size; // image size in pixels (width, height) regardless of its format
  PixelFormat _format;      // image format
  int _strides[4];          // row strides
  utils::ulong _offsets[4]; // surface offsets in the array

  utils::ullong _timestamp; // timestamp of this frame when captured

  utils::byte *_pixels;  // pointer to the data array
  utils::uint _datasize; // size of the data array
  bool _autoDelete;      // automatically delete the pixels when destroyed

  bool _shmem;               // is it a shared memory array?
  utils::ulong _shmemOffset; // the offset of the shared memory from the pointer
                             // to be taken to consideration
  void _DeleteSafe();        // delete the pixels safely
public:
  VideoFrame();
  ~VideoFrame();
  /**
   * @brief Set the Shared Memory Offset, only required if (this) is pointing to
   * a shared memory. Mainly used by video resources
   *
   * @param shmemOffset bytes offset
   */
  void SetSharedMemoryOffset(utils::ulong shmemOffset);

  /**
   * @brief Set the Shared Memory pointer of the samples
   *
   * @param ptr
   */
  void SetSharedMemoryPtr(utils::byte *ptr);

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
   * @brief  override the internal samples data array with the provided one,
   * assuming the data size is the same as the one insde. Also, provide the
   * option to delete the internal one, and whether or not to auto delete the
   * pointer when finished using it
   *
   * @param ptr bytes array pointer
   * @param deleteInternal delete the internal data before replacing it
   * @param autoDelete auto delete the array pointer when finished using it
   * (destructor)
   */
  void SetSamplesPtr(utils::byte *ptr, bool deleteInternal, bool autoDelete);

  /**
   * @brief Get Image Format
   *
   * @return PixelFormat
   */
  PixelFormat GetFormat() const { return _format; }
  /**
   * @brief Returns the size of the image frame in pixels (Width, Height)
   *
   * @return const utils::vector2di&
   */
  const utils::vector2di &GetSize() const { return _size; }

  /**
   * @brief Total data size of the image data array
   *
   * @return utils::uint
   */
  utils::uint GetDataSize() const { return _datasize; }

  /**
   * @brief Returns the data array pointer, it manages whether its from a shared
   * memory or dedicated memory.
   *
   * @return utils::byte*
   */
  utils::byte *GetSamplesPtr();
  const utils::byte *GetSamplesPtr() const;

  /**
   * @brief Initialize the video frame object, creating internal memory using
   * provided information about the requested video frame specs
   *
   * @param width Width in pixels
   * @param height Height in pixels
   * @param strides Four value array describing the strides of the pixel
   * surfaces (i.e YUYV planes)
   * @param offsets Four value array describing the offsets of the pixel
   * surfaces (i.e YUYV planes)
   * @param format format of the pixel
   * @param datasize total data size to be allocated, if set to zero then it
   * will try to auto calculate the data size
   */
  void InitFrame(int width, int height, const int *strides,
                 const utils::ulong *offsets, PixelFormat format,
                 utils::uint datasize);
  /**
   * @brief Minimal way to initalize a video frame using size and format
   *
   * @param width
   * @param height
   * @param format
   */
  void InitFrame(int width, int height, PixelFormat format);

  /**
   * @brief Initialize and copy an existing pixel array
   *
   * @param width
   * @param height
   * @param format
   * @param data
   * @param copy
   */
  void InitFrame(int width, int height, PixelFormat format,
                 const utils::byte *data, bool copy = true);

  /**
   * @brief Clone this video frame to another frame
   *
   * @param to
   * @return VideoFrame*
   */
  VideoFrame *Clone(VideoFrame *to, utils::byte *shmemPtr = 0) const;
};

} // namespace media

} // namespace avatarflow

#endif //__VIDEOFRAME__