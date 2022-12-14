

#ifndef ___RANDOMIZER___
#define ___RANDOMIZER___
#include "avatarutils_config.h"

namespace utils {

class AvaUtils_API Randomizer {
    static int _seed;

   public:
    static void reset();
    static void srand(int seed);
    static int rand();
    static int rand(int range);
    static float rand(int range, int per);
    static float rand01();
    static float randRange(float low, float high);
    static int RouletteWheelSelection(float *slices, int cnt, float maxRange);
};

};  // namespace utils

#endif
