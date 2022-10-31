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

#ifndef __SHAREDMEMORYRESOURCEMANAGER__
#define __SHAREDMEMORYRESOURCEMANAGER__

#include <utils/ListenerContainer.h>

#include "CoreResource.h"
#include "flow/avatarflow_config.h"
/////
#include "flow/media/Data/IDataChannelManagerListener.h"

namespace avatarflow {
namespace media {
class AudioFrame;
class DataFrame;
class VideoFrame;
class DataChannelManager;

class ResourceManagerImpl;

/**
 * @brief Helper class that provides encapsulation for all data resources
 * captured by the resource manager, and fired as a single resource packet.
 *
 * It contains information about the Packet number, capturing frequency, and
 * three dictionaries for video, audio, and data resources assigned by the
 * resource manager
 */
class FullResourcePacket {
public:
  /** \brief Packet ID, sequential number indicating the packet sequence  */
  int ID;
  /** \brief Packets per second, capturing frequency */
  int frequency;

  /** \brief  Dictionary containing the video resources provided by the
   * resource manager*/
  std::map<std::string, VideoResourcePacket> video;
  /** \brief  Dictionary containing the audio resources provided by the
   * resource manager*/
  std::map<std::string, AudioResourcePacket> audio;
  /** \brief  Dictionary containing the data resources provided by the
   * resource manager*/
  std::map<std::string, DataResourcePacket> data;
};

/**
 * @brief Resource Listener class, override the function to get callbacks from
 * resource manager when new frames arrive
 *
 */
class IResourceListener {
public:
  virtual ~IResourceListener() {}

  /**
   * @brief Video frame callback, it fires this function with the frame
   * contents at the frequency it was set to
   *
   * @param frame: Video frame data
   * @param newFrame: True if its a new frame, false otherwise
   * @param shmem: name of the resource
   */
  virtual void Resource_OnVideoFrame(const media::VideoFrame *frame,
                                     bool newFrame, const std::string &shmem) {}

  /**
   * @brief Audio frame callback, it fires this function with the frame
   * contents at the frequency it was set to
   *
   * @param frame: Audio frame data
   * @param newFrame: True if its a new frame, false otherwise
   * @param shmem: name of the resource
   */
  virtual void Resource_OnAudioFrame(const media::AudioFrame *frame,
                                     bool newFrame, const std::string &shmem) {}

  /**
   * @brief Data frame callback, it fires this function with the frame
   * contents at the frequency it was set to
   *
   * @param frame: Data frame data
   * @param newFrame: True if its a new frame, false otherwise
   * @param shmem: name of the resource
   * @param channelName: name of the channel of this resource
   */
  virtual void Resource_OnDataFrame(const media::DataFrame *frame,
                                    bool newFrame, const std::string &shmem,
                                    const std::string &channelName) {}
  /**
   * @brief Data frame callback, it fires this function with the frame
   * contents at the frequency it was set to
   *
   * @param frame: Data frame data
   * @param newFrame: True if its a new frame, false otherwise
   * @param shmem: name of the resource
   * @param channelName: name of the channel of this resource
   */
  virtual void Resource_OnFullPacket(const FullResourcePacket &packet) {}
};

/**
 * @brief Helper class to access the shared resources for read/write in a
 * transparent way. This class automatically creates/access the shared memory
 * objects, and maintains an internal state for them to avoid recreation of the
 * resources.
 *
 */
class AvaFlow_API ResourceManager
    : public utils::ListenerContainer<IResourceListener *>,
      public IDataChannelManagerListener {
private:
  ResourceManagerImpl *_impl;

public:
  DECLARE_FIRE_METHOD(Resource_OnVideoFrame,
                      (const media::VideoFrame *frame, bool newFrame,
                       const std::string &shmem),
                      (frame, newFrame, shmem))

  DECLARE_FIRE_METHOD(Resource_OnAudioFrame,
                      (const media::AudioFrame *frame, bool newFrame,
                       const std::string &shmem),
                      (frame, newFrame, shmem))

  DECLARE_FIRE_METHOD(Resource_OnDataFrame,
                      (const media::DataFrame *frame, bool newFrame,
                       const std::string &shmem,
                       const std::string &channelName),
                      (frame, newFrame, shmem, channelName))

  DECLARE_FIRE_METHOD(Resource_OnFullPacket, (const FullResourcePacket &packet),
                      (packet))

  virtual void OnNewDataArrived(DataChannelManager *mngr, IDataChannel *channel,
                                const media::DataFrame *frame);

public:
  ResourceManager(/* args */);
  ~ResourceManager();

  /**
   * @brief Subscribe to a video resource, the name of resource should be
   * provided
   *
   * @param shmem: Resource name to subscribe into
   */
  void SubscribeToVideo(const std::string &shmem);

  void UnsubscribeToVideo(const std::string &shmem);

  /**
   * @brief Subscribe to a audio resource, the name of resource should be
   * provided
   *
   * @param shmem: Resource name to subscribe into
   */
  void SubscribeToAudio(const std::string &shmem);
  void UnsubscribeToAudio(const std::string &shmem);

  /**
   * @brief Subscribe to a data resource, both the name of resource and
   * channel name should be provided
   *
   * @param shmem: Resource name to subscribe into
   * @param channelName: Channel name of this data
   */
  void SubscribeToData(const std::string &shmem,
                       const std::string &channelName);
  void UnsubscribeToData(const std::string &shmem,
                         const std::string &channelName);

  void ClearVideoSubscribers();
  void ClearAudioSubscribers();
  void ClearDataSubscribers();

  /**
   * @brief Start Resource fetching, only for the subscribed scenario
   *
   * @param fetchms fetching time in millisecond, lower means more frequent
   * fetches but also more CPU usage
   */
  void Start(int fetchms = 10);
  /**
   * @brief Start Resource fetching but only asynchronusly. This will trigger
   * only when new resources are available
   */
  void StartAsync();

  /**
   * @brief Stops notifiying about new resources
   *
   */
  void Stop();

  /**
   * @brief Retrives a video frame for a shared resource
   *
   * @param shmem Resource Name
   * @param isNewFrame True if the resource is new
   * @return const media::VideoFrame* last video frame in this resource
   */
  const media::VideoFrame *GetVideoFrame(const std::string &shmem,
                                         bool &isNewFrame);

  /**
   * @brief clean way to get video frame packet
   *
   * @param packet
   * @return true
   * @return false
   */
  bool GetVideoFrame(VideoResourcePacket *packet);

  /**
   * @brief Retrives a audio frame for a shared resource
   *
   * @param shmem Resource Name
   * @param isNewFrame True if the resource is new
   * @return const media::VideoFrame* last audio frame in this resource
   */
  const media::AudioFrame *GetAudioFrame(const std::string &shmem,
                                         bool &isNewFrame);

  /**
   * @brief clean way to get audio frame packet
   *
   * @param packet
   * @return true
   * @return false
   */
  bool GetAudioFrame(AudioResourcePacket *packet);

  /**
   * @brief Retrives a data frame for a shared resource and channel name
   *
   * @param shmem Resource Name
   * @param channelName channel name
   * @param isNewFrame True if the resource is new
   * @return const media::DataFrame* last data frame in this resource/channel
   */

  const media::DataFrame *GetDataFrame(const std::string &shmem,
                                       const std::string &channelName,
                                       bool &isNewFrame);

  /**
   * @brief clean way to get data frame packet
   *
   * @param packet
   * @return true
   * @return false
   */
  bool GetDataFrame(DataResourcePacket *packet);

  /**
   * @brief Update a video resource with a new frame
   *
   * @param shmem: Name of this resource (shared memory)
   * @param frame: video contents of this resource
   * @return true
   * @return false
   */
  bool WriteVideoFrame(const std::string &shmem,
                       const media::VideoFrame *frame);

  /**
   * @brief  Update an audio resource with a new frame
   *
   * @param shmem: Name of this resource (shared memory)
   * @param frame: audio contents of this resource
   * @return true
   * @return false
   */
  bool WriteAudioFrame(const std::string &shmem,
                       const media::AudioFrame *frame);

  /**
   * @brief  Update a data resource with a new frame
   *
   * @param shmem: Name of this resource (shared memory)
   * @param channelName: Target channel to write into
   * @param frame: data contents of this resource
   * @return true
   * @return false
   */
  bool WriteDataFrame(const std::string &shmem, const std::string &channelName,
                      const media::DataFrame *frame);

  /**
   * @brief  Update a data resource with a new frame using direct binary data
   *
   * @param shmem: Name of this resource (shared memory)
   * @param channelName: Target channel to write into
   * @param data: binary data pointer
   * @param len: length of data to write
   * @return true
   * @return false
   */
  bool WriteDataBinary(const std::string &shmem, const std::string &channelName,
                       const void *data, unsigned int len);

  /**
   * @brief  Update a data resource with a new frame using direct string data
   *
   * @param shmem: Name of this resource (shared memory)
   * @param channelName: Target channel to write into
   * @param data: string data
   * @return true
   * @return false
   */
  bool WriteDataString(const std::string &shmem, const std::string &channelName,
                       const std::string &data);
};

} // namespace media
} // namespace avatarflow

#endif //__SHAREDMEMORYRESOURCEMANAGER__