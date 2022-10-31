#ifndef VIDEO_WRAPPER_H
#define VIDEO_WRAPPER_H

#include <flow/media/Resources/VideoFrame.h>
#include <gst/gst.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <flow/mobile/MobileDefs.h>
#include <flow/services/IService.h>
#include <flow/media/GStreamer/GstPipeline.h>

namespace avatarflow {
    namespace wrapper {
        namespace android {

            class IGSTDataCallback{
            public:
                virtual void hasNewFrame(const media::VideoFrame *frame) = 0;
            };

            class IWrapperImp;

            class VideoWrapper {
            public:
                VideoWrapper(IGSTDataCallback *cb, media::GstPipeline* pipeline, const std::string &sinkName, RenderType renderType, ANativeWindow *view = NULL);

                ~VideoWrapper();

                bool init(services::IService *bindService = NULL);

                void play();

                void pause();

                void stop(bool stopService);

                void setWindow(ANativeWindow * view);

                void exposeVideoOverlay();

            private:
                IWrapperImp* wrapperImp;

                services::IService *mBoundService;

            public:
                media::GstPipeline* mPipeline;

                RenderType mRenderType;

                std::string mSinkName;
            };
        }
    }
}

#endif
