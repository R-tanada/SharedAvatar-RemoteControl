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

#ifndef __ShmemDataAppSinkHandler__
#define __ShmemDataAppSinkHandler__

#include <utils/atypes.h>

#include "../AppSrc/IDataGrabber.h"
#include "../Resources/CoreResource.h"
#include "../Resources/DataFrame.h"
#include "IAppSinkHandler.h"

namespace avatarflow {
namespace media {

class ShmemDataAppSinkHandlerImpl;
class AvaFlow_API ShmemDataAppSinkHandler : public IDataGrabber,
                                            public IAppSinkHandler {
public:
protected:
  ShmemDataAppSinkHandlerImpl *_DataImpl;

public:
  ShmemDataAppSinkHandler();
  virtual ~ShmemDataAppSinkHandler();

  void SetName(const std::string &name, utils::uint size,
               CoreResourceProvider provider = Provider_Unkown);

  virtual void SetSink(void *s);

  virtual void Close();

  virtual bool GrabFrame();
  virtual bool HasNewFrame() const;
  virtual utils::uint GetFrameLength() const;

  virtual const DataFrame *GetLastFrame() const;

  virtual int process_sample(void *sample);
};

} // namespace media

} // namespace avatarflow

#endif //__ShmemDataAppSinkHandler__