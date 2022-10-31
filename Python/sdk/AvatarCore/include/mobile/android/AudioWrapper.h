#ifndef AUDIO_WRAPPER_H
#define AUDIO_WRAPPER_H

#include <flow/services/IService.h>

namespace avatarflow {
    namespace wrapper {
        namespace android {
            class AudioWrapper {
            public:
                AudioWrapper();

                ~AudioWrapper();

                bool init(services::IService *bindService = NULL);

                void play();

                void pause();

                void stop();

            private:

                services::IService *mBoundService;
            };
        }
    }
}

#endif
