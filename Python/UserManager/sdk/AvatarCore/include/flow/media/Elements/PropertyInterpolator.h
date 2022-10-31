

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

#ifndef __PROPERTYINTERPOLATOR__
#define __PROPERTYINTERPOLATOR__

#include "flow/avatarflow_config.h"
#include <utils/Tween.h>
#include <utils/atypes.h>

namespace avatarflow {
namespace media {

class AvaFlow_API PropertyInterpolator {
   private:
    void* _element;
    std::string _propName;
    float _targetValue;

    utils::DTween _tween;

   public:
    PropertyInterpolator(/* args */);
    ~PropertyInterpolator();

    void Attach(void* elem, const std::string& propName, float initialValue);

    void SetTargetValue(float value);

    void Update(float dt);
};

}  // namespace media
}  // namespace avatarflow

#endif  //__PROPERTYINTERPOLATOR__