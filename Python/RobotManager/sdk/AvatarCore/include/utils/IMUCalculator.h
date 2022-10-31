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

#ifndef __IMUCALCULATOR__
#define __IMUCALCULATOR__

#include "JSONUtils.h"
#include "point3D.h"

namespace utils {
class IMUCalculator {
private:
  utils::vector3d _euler;
  utils::vector3d _eulerMapped;
  int _calibSamplesCount;
  utils::vector3d _gyroBias;
  utils::vector3d _gravityCalib;
  float _gravity;
  float _gyroCoeff;

  bool _isFirst;

  float _wrap(float angle, float limit);

public:
  IMUCalculator(float gyroCoeff = 0.98);
  ~IMUCalculator();

  void Reset();

  void Update(const utils::vector3d &accel, const utils::vector3d &gyro,
              const utils::vector3d &mag, float dt);

  const utils::vector3d &GetEuler();

  bool IsReady();

  JSONObject _save();
  bool _load(const JSONObject &json);
};

} // namespace utils

#endif //__IMUCALCULATOR__