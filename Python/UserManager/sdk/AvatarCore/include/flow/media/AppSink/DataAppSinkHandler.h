
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

#ifndef __DataAPPSINKHANDLER__
#define __DataAPPSINKHANDLER__

#include "../AppSrc/IDataGrabber.h"
#include "IAppSinkHandler.h"

namespace avatarflow {
namespace media {

class DataAppSinkHandlerImpl;
class AvaFlow_API DataAppSinkHandler : public IDataGrabber,
                                       public IAppSinkHandler {
public:
protected:
  DataAppSinkHandlerImpl *_DataImpl;

public:
  DataAppSinkHandler();
  virtual ~DataAppSinkHandler();

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

#endif // DataAppSinkHandler_h__
