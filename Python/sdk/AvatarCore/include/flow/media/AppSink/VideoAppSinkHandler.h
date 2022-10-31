
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

#ifndef __VIDEOAPPSINKHANDLER__
#define __VIDEOAPPSINKHANDLER__

#include "../AppSrc/IVideoGrabber.h"
#include "../Resources/VideoFrame.h"
#include "IAppSinkHandler.h"
#include "OS/FPSCalc.h"

namespace avatarflow {
namespace media {

class VideoAppSinkHandlerImpl;
class AvaFlow_API VideoAppSinkHandler : public IVideoGrabber,
                                        public IAppSinkHandler {
   public:
   protected:
    VideoAppSinkHandlerImpl *_videoImpl;

   public:
    VideoAppSinkHandler();
    virtual ~VideoAppSinkHandler();

    virtual void SetSink(void *s);

    virtual const utils::vector2di &GetFrameSize();
    virtual PixelFormat GetImageFormat();

    virtual void Close();
    virtual bool HasNewFrame();
    virtual int GetFramesCount();
    virtual bool GrabFrame();
    virtual utils::ulong GetBufferID();
    float GetCaptureFrameRate();

    const VideoFrame *GetLastFrame() const;

    virtual int process_sample(void *sample);
};

}  // namespace media
}  // namespace avatarflow

#endif  // VideoAppSinkHandler_h__
