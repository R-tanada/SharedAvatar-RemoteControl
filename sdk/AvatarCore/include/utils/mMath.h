
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

#ifndef __MMATH__
#define __MMATH__

#include <float.h>
#include <math.h>

#include "avatarutils_config.h"

namespace utils {

class AvaUtils_API MTrigTable {
    static bool m_initlized;
    static unsigned short m_trigTableSize;
    static double m_trigTableFactor;

    static double *m_sinTable;
    static double *m_tanTable;

    static double sinTable(double angle);
    static double tanTable(double angle);

    static bool m_useTable;
    void buildTables();

   public:
    MTrigTable(unsigned short tableSize = 4096);
    ~MTrigTable();

    static void useTable(bool use);
    static bool isUseTable();

    static inline double sin(double angle) { return sinTable(angle); }
    static double cos(double angle);
    static inline double tan(double angle) { return tanTable(angle); }
};

class AvaUtils_API Math {
   public:
    static const float Epsilon;
    static const double Zero;
    static const float Infinity;

    static const float RoundingValue;
    static const double PI64;
    static const double TwoPI64;
    static const double HalfPI64;
    static const double OneFourthPI64;

    static const float PI32;
    static const float TwoPI32;
    static const float HalfPI32;
    static const float OneFourthPI32;

#define RANDOM_FLOAT ((float)rand() / RAND_MAX)

    static const double GRAD_PI64;
    static const float GRAD_PI32;

    static const float i255;

    static inline double toRad(double a) { return a * PI64 / 180; }
    static inline double toDeg(double a) { return a * 180 / PI64; }

    static inline double cosd(double a) { return MTrigTable::cos(toRad(a)); }
    static inline double sind(double a) { return MTrigTable::sin(toRad(a)); }
    static inline double tand(double a) { return MTrigTable::tan(toRad(a)); }

    static inline double cosr(double a) { return MTrigTable::cos(a); }
    static inline double sinr(double a) { return MTrigTable::sin(a); }
    static inline double tanr(double a) { return MTrigTable::tan(a); }

    template <class T>
    static inline T Max(const T a, const T b) {
        return a < b ? b : a;
    }
    template <class T>
    static inline T Min(const T a, const T b) {
        return a < b ? a : b;
    }

    template <class T>
    static inline T sqr(const T a) {
        return a * a;
    }

    static inline bool isZero(float val) {
        return (val < utils::Math::Zero && val > -utils::Math::Zero);
    }

    template <class T>
    static inline int sign(T x) {
        return x < 0 ? -1 : 1;
    }
    /*
    float roundNumber(float x,float a,float b)
    {
            bool flag=0;
            if(b==0)
            {
                    flag=1;
                    b+=10;
                    a+=10;
            }
            if(x<a)
            {
                    float tmp=a-x;
                    tmp=tmp-b*(int)(tmp/b);
                    x=b-tmp;
            }
            if(x>=b)
                    x=x-b*(int)(x/b);
            if(flag)
                    x-=10;
            return x;
    }
    */
    template <class T>
    static void Swap(T &a, T &b) {
        T tmp;
        tmp = a;
        a = b;
        b = tmp;
    }

    static inline bool fEqual(float v1, float v2,
                              float epsilon = utils::Math::RoundingValue) {
        return (v1 + epsilon > v2 && v1 - epsilon < v2);
    }

    template <class T>
    static inline T lerp(T a, T b, float t) {
        return (T)(a * -(t - 1)) + b * (t);
    }
    template <class T>
    static inline T clamp(T x, T low, T high) {
        return Min(Max(x, low), high);
    }
    template <class T>
    static inline T cycle(T x, T a, T b) {
        return (x - a) < 0 ? (b + x - a) : (x >= b) ? (x - b + a) : x;
    }
    template <class T>
    static inline T ang(T a) {
        a = fmod((double)a, (double)360);
        int s = a >= 0 ? +1 : -1;
        if (fabs((double)a) > 180) return a - s * 360;
        return a;
    }

    template <class T>
    static bool getValFromFrame(T *arr, int arrSize, float frame, int sframe,
                                int eframe, T &out) {
        if (!arr || frame < 0 || sframe < 0 || eframe < 0 ||
            sframe >= arrSize || eframe >= arrSize)
            return false;
        float Alpha = frame / 100.0f;
        float AlphaInv = 1 - Alpha;

        out = arr[eframe] * Alpha + arr[sframe] * AlphaInv;
        return 1;
    }

    template <class T>
    static bool getBlendValFromFrame(T *arr, int arrSize, float frame,
                                     int sframe, int eframe, float blendAmount,
                                     int blendFrame, T &out) {
        if (!arr || frame < 0 || sframe < 0 || eframe < 0 || blendFrame < 0 ||
            sframe >= arrSize || eframe >= arrSize || blendFrame >= arrSize ||
            blendAmount < 0)
            return false;
        float BAlpha = blendAmount / 100.0f;
        float BAlphaInv = 1 - BAlpha;

        float FAlpha = frame / 100.0f;
        float FAlphaInv = 1 - FAlpha;

        out = (arr[eframe] * FAlpha + arr[sframe] * FAlphaInv) * BAlphaInv +
              arr[blendFrame] * BAlpha;
        return 1;
    }
};

}  // namespace utils

#endif