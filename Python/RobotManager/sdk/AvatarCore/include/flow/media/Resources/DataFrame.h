

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

#ifndef __DataFRAME__
#define __DataFRAME__

#include "flow/avatarflow_config.h"
#include "utils/atypes.h"

namespace avatarflow {
namespace media {

/**
 * @brief This class contains an array of bytes representing a generic data
 * frame (bytes array). \n It can be pointing to a shared memory, and thus it
 * will manage not to destroy when the object is deleted
 */
class AvaFlow_API DataFrame {
public:
  /***** Internal Data, only use when its a must!! *****/
  utils::byte *_data;
  utils::uint _length;
  bool _autoDelete; // automatically delete the pixels when destroyed

  utils::uint _poolSize;
  bool _shmem;
  utils::ullong _timestamp;
  utils::ulong _shmemOffset;
  void _DeleteSafe(); // delete the pixels safely

public:
  DataFrame(/* args */);
  ~DataFrame();
  void SetSharedMemoryOffset(utils::ulong shmemOffset);
  void SetSharedMemoryPtr(utils::byte *ptr);

  /**
   * @brief Get the data pointer of the samples contained in this object, same
   * as GetData. Use this method to retrieve the data as it will make all
   * necessary checks regarding the validaty of the data object as well as
   * protection for shared memory data scenario.
   *
   * @return utils::byte*
   */
  utils::byte *GetSamplesPtr();
  const utils::byte *GetSamplesPtr() const;
  const utils::byte *GetData() const;
  utils::byte *GetData();

  /**
   * @brief Get the Length of data in bytes
   *
   * @return utils::uint
   */
  utils::uint GetLength() const;
  utils::uint GetLength();

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
   * @brief Initialize the data frame object, creating necessary memory pool for
   * the length provided
   *
   * @param length: requested length of data to be reserved
   * @param hardReset: if set to true, then it will delete the internal data
   * even if the length is matching the requested one
   */
  void InitFrame(utils::uint length, bool hardReset = false);
  /**
   * @brief Initialize the data frame object from existing data, the data will
   * be copied (data pool will be created)
   *
   * @param length: size of data passed
   * @param ptr: pointer of data object to copy data from
   * @param hardReset: if set to true, then it will delete the internal data
   * even if the length is matching the requested one
   */
  void InitFrame(utils::uint length, const void *ptr, bool hardReset = false);

  /**
   * @brief Sets the data from existing data object, will not create a memory
   * but will use the data pointer passed as data object (encapsulation)
   *
   * @param length
   * @param ptr
   */
  void InitFrameUsingData(utils::uint length, const void *ptr);

  /**
   * @brief Clone this data frame to another data frame
   *
   * @param to
   * @return DataFrame*
   */
  DataFrame *Clone(DataFrame *to) const;
};

} // namespace media
} // namespace avatarflow

#endif //__DataFRAME__