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

#ifndef __IResourceHeader__
#define __IResourceHeader__

#include "CoreResource.h"

namespace avatarflow {
namespace media {

/**
 * @brief a common header for the resources used in the core, it manages meta
 * data about the resource when its attempted to be accessed for referencing
 *
 */
class IResourceHeader {
private:
  CoreResourceType _type;

protected:
  CoreResourceProvider _provider;
  utils::ulong _flags;

  utils::ulong _frameCounter;
  int _refCounter;

public:
  IResourceHeader(CoreResourceType type,
                  CoreResourceProvider provider = Provider_Local)
      : _frameCounter(0), _type(type), _provider(provider), _flags(0),
        _refCounter(0) {}

  CoreResourceType GetType() { return _type; }

  void SetProvider(CoreResourceProvider p) { _provider = p; }
  CoreResourceProvider GetProvider() { return _provider; }

  void ResetResourceVersion() { _frameCounter = 0; }
  void UpdateResourceVersion() { _frameCounter++; }
  utils::ulong GetResourceVersion() { return _frameCounter; }

  int GetRefCounter() { return _refCounter; }
  void Ref() { ++_refCounter; }
  // return true if the ref counter reach 0
  bool Unref() {
    --_refCounter;
    if (_refCounter == 0)
      return true;
    return false;
  }
  int GetRefCount() { return _refCounter; }
};

} // namespace media
} // namespace avatarflow

#endif //__IResourceHeader__