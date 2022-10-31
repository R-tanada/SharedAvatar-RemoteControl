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

#ifndef __TWEEN__
#define __TWEEN__

#include "mMath.h"

namespace utils {

/// Exponential interpolation
class ETween {
   public:
    static float Step(float current, float target, float omega, float dt) {
        float exp = ::exp(-omega * dt);
        return Math::lerp(target, current, exp);
    }
};

/// Interpolation with critically damped spring model
class DTween {
   public:
    static float Step(float current, float target, float& velocity, float omega,
                      float dt) {
        float n1 = velocity - (current - target) * (omega * omega * dt);
        float n2 = 1 + omega * dt;
        velocity = n1 / (n2 * n2);
        return current + velocity * dt;
    }

   public:
    float position;
    float velocity;
    float omega;

   public:
    DTween() : position(0), omega(1) {}
    DTween(float position, float omega) {
        this->position = position;
        this->velocity = 0;
        this->omega = omega;
    }

    float Step(float target, float dt) {
        position = Step(position, target, velocity, omega, dt);
        return position;
    }
};

}  // namespace utils

#endif  //__TWEEN__