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

#ifndef __MOVINGAVERAGE__
#define __MOVINGAVERAGE__

namespace utils {
class MovingAverage {
private:
  /* data */

  int _maxSamples;
  float *_valuesVector;
  int _currSize;
  int _currIndex;

  float _currSum;

public:
  MovingAverage(int MaxSamples) : _maxSamples(MaxSamples) {
    _valuesVector = 0;
    _currSize = 0;
    _currIndex = 0;
    _currSum = 0;

    if (_maxSamples > 0) {
      _valuesVector = new float[_maxSamples];
    }
  }
  ~MovingAverage() { delete _valuesVector; }

  float AddSample(float v) {
    if (_currIndex < _currSize)
      _currSum -= _valuesVector[_currIndex];
    _valuesVector[_currIndex++] = v;
    if (_currSize < _currIndex) {
      _currSize = _currIndex;
    }
    _currSum += v;
    _currIndex %= _maxSamples;

    if (_currSum == 0)
      return 0;

    return _currSum / (float)_currSize;
  }

  void Reset() {
    _currSize = 0;
    _currIndex = 0;
    _currSum = 0;
  }

  float GetAverage() { return _currSum / (float)_currSize; }
};

} // namespace utils

#endif //__MOVINGAVERAGE__