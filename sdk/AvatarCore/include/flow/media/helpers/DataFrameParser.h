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

#ifndef __DATAFRAMEPARSER__
#define __DATAFRAMEPARSER__

#include <utils/GUID.h>
#include <utils/atypes.h>

#include "flow/media/Resources/DataFrame.h"

namespace avatarflow {
namespace media {

/**
 * @brief Parser Writer class to DataFrame object
 *
 */
class AvaFlow_API DataFrameParser {
private:
  void *_data;
  utils::uint _length;
  utils::uint _offset;

public:
  DataFrameParser(/* args */);
  ~DataFrameParser();

  /**
   * @brief Set the Data Frame object to parse from, and resets internal parse
   * offset
   *
   * @param frame
   */
  void SetDataFrame(DataFrame *frame);

  /**
   * @brief Set the Data object directly via a native pointer array
   *
   * @param data
   * @param length
   */
  void SetData(void *data, utils::uint length);
  /**
   * @brief Get the Offset of the data (also represents the total size of the
   * data written so far)
   *
   * @return uint
   */
  utils::uint GetOffset() { return _offset; }

  /**
   * @brief Reset parsing offset to beginning
   *
   */
  void Reset();
  /**
   * @brief Parse from the attached dataframe number of bytes and set them to
   * data object
   *
   * @param data target location to store the parsed data
   * @param bytes number of data to parse
   * @return true parse successed
   * @return false failed to parse the number of bytes (out of length)
   */
  bool Parse(void *data, int bytes);

  /**
   * @brief Write number of bytes to the dataframe object
   *
   * @param data source bytes to copy from
   * @param bytes number of bytes to copy
   * @return true writing successful
   * @return false failed to write due to lack of spacce
   */
  bool Write(const void *data, int bytes);

  /**
   * @brief Parse string array, this will handle the length of the string
   *
   * @param str
   * @return true
   * @return false
   */
  bool Parse(std::string &str);
  /**
   * @brief Write a string to the data frame. This will handle the length of
   * the string
   *
   * @param str
   * @return true
   * @return false
   */
  bool Write(const std::string &str);

  bool Parse(utils::GUID &str);
  bool Write(const utils::GUID &str);
  /**
   * @brief Parse a generic array, this will handle the length of the array
   *
   * @param arr
   * @return true
   * @return false
   */
  template <class T> bool Parse(std::vector<T> &arr) {
    utils::uint len;
    if (!Parse(&len, sizeof(len)))
      return false;
    arr.resize(len);
    if (len == 0)
      return true;
    return Parse((char *)&arr[0], len * sizeof(T));
  }

  /**
   * @brief Write into a generic array, this will handle the length of the
   * array
   *
   * @param arr
   * @return true
   * @return false
   */
  template <class T> bool Write(const std::vector<T> &arr) {
    utils::uint len = arr.size();
    if (!_data || _offset + (sizeof(len) + len) > _length)
      return false;
    Write(&len, sizeof(len));
    if (len > 0)
      Write((char *)&arr[0], len * sizeof(T));
    return true;
  }
  /**
   * @brief Generic parser, Performs shallow copy into the data,
   *  requires data not to have deep representation.
   *
   * @tparam T: Type of data
   * @param data: Data storage
   * @return true
   * @return false
   */
  template <class T> bool Parse(T &data) {
    return Parse(&data, (int)(sizeof(T)));
  }

  /**
   * @brief Generic writer, Performs shallow copy from the data,
   *  requires data not to have deep representation.
   *
   * @tparam T: Type of data
   * @param data: Data storage
   * @return true
   * @return false
   */
  template <class T> bool Write(const T &data) {
    return Write((const void *)&data, (int)(sizeof(T)));
  }

  template <class T> DataFrameParser &operator>>(T &dt) {
    Parse(dt);
    return *this;
  }
  template <class T> DataFrameParser &operator<<(const T &dt) {
    Write(dt);
    return *this;
  }
};

} // namespace media
} // namespace avatarflow

#endif //__DATAFRAMEPARSER__