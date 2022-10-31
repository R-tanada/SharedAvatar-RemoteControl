#ifndef AVATAR_MESSAGE_HELPER_H
#define AVATAR_MESSAGE_HELPER_H

#include <mobile/DataWrap.h>

namespace avatarmobile
{
    class AvatarMessageHelperImpl;

    class AvatarMessageHelper
    {
    private:
        AvatarMessageHelperImpl* _impl;
    public:
        
        // To avoid repeated if else
        float  forwardMove;
        float  strideMove;
        float  neckMove;
        
        
        int  batteryLevel;
        bool isCharging;
    public:
        AvatarMessageHelper(DataWrap*d);
        ~AvatarMessageHelper();


        void sendAvatarIn();
        void updateMove();
        void updateNeck();
        std::string parseJson (const std::string& json);
        
    };

} // namespace avatarmobile

#endif
