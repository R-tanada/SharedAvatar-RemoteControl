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

#ifndef __MediaSharedMemoryManager__
#define __MediaSharedMemoryManager__

#include <utils/GUID.h>
#include <utils/ISingleton.h>

#include "../Resources/CoreResource.h"
#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class SharedMemoryAudioGrabber;
class SharedMemoryAudioWriter;

class SharedMemoryVideoGrabber;
class SharedMemoryVideoWriter;

class SharedMemoryDataGrabber;
class SharedMemoryDataWriter;

class MediaSharedMemoryManagerImpl;
class AvaFlow_API MediaSharedMemoryManager
    : public utils::ISingleton<MediaSharedMemoryManager> {
public:
  static const utils::GUID PATH_SharedMemoryCreated;
  static const utils::GUID PATH_SharedMemoryClosed;

private:
  MediaSharedMemoryManagerImpl *_impl;

public:
  MediaSharedMemoryManager(/* args */);
  virtual ~MediaSharedMemoryManager();

  SharedMemoryAudioGrabber *GetAudioMemory(const std::string &name,
                                           bool &created, bool force = false);
  SharedMemoryAudioWriter *CreateAudioMemory(
      const std::string &name, utils::uint length, bool force = false,
      CoreResourceProvider provider = CoreResourceProvider::Provider_Unkown);

  void RemoveAudioGrabberMemory(SharedMemoryAudioGrabber *mem);
  void RemoveAudioWriterMemory(SharedMemoryAudioWriter *mem);

  SharedMemoryVideoGrabber *GetVideoMemory(const std::string &name,
                                           bool &created, bool force = false);
  SharedMemoryVideoWriter *CreateVideoMemory(
      const std::string &name, utils::uint length, bool force = false,
      CoreResourceProvider provider = CoreResourceProvider::Provider_Unkown);

  void RemoveVideoGrabberMemory(SharedMemoryVideoGrabber *mem);
  void RemoveVideoWriterMemory(SharedMemoryVideoWriter *mem);

  SharedMemoryDataGrabber *GetDataMemory(const std::string &name, bool &created,
                                         bool force = false);
  SharedMemoryDataWriter *CreateDataMemory(
      const std::string &name, utils::uint length, bool force = false,
      CoreResourceProvider provider = CoreResourceProvider::Provider_Unkown);
  void RemoveDataGrabberMemory(SharedMemoryDataGrabber *mem);
  void RemoveDataWriterMemory(SharedMemoryDataWriter *mem);
};

#define gMediaSharedMemoryManager                                              \
  avatarflow::media::MediaSharedMemoryManager::getInstance()

} // namespace media
} // namespace avatarflow

#endif //__MediaSharedMemoryManager__