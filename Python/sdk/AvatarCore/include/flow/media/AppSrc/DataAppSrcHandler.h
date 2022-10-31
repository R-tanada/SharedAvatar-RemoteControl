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

#ifndef __DataAPPSRCHANDLER__
#define __DataAPPSRCHANDLER__

#include "IAppSrcHandler.h"

namespace avatarflow {
namespace media {

class IDataGrabber;

class DataAppSrcHandlerImpl;
class AvaFlow_API DataAppSrcHandler : public IAppSrcHandler {
private:
  DataAppSrcHandlerImpl *_impl;

public:
  DataAppSrcHandler(const std::string &name);
  ~DataAppSrcHandler();

  virtual void SetSrc(void *s, GstPipeline *pipeline, unsigned int rate,
                      bool pullMode = false);

  virtual void AttachDataGrabber(IDataGrabber *g);
  void Stop();
};

} // namespace media

} // namespace avatarflow

#endif //__DataAPPSRCHANDLER__