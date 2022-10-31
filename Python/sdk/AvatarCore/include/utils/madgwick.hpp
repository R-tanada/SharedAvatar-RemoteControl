/************************************************************************
 *                                                                      *
 * Copyright 2021 AvatarIn Inc. Tokyo, Japan. All Rights Reserved       *
 *                                                                      *
 *       _____               __               .___                      *
 *      /  _  \___  ______ _/  |______ _______|   | ____                *
 *     /  /_\  \  \/ |__  \\   __\__  \\_  __ \   |/    \               *
 *    /    |    \   / / __ \|  |  / __ \|  | \/   |   |  \              *
 *    \____|__  /\_/ (____  /__| (____  /__|  |___|___|  /              *
 *            \/          \/          \/               \/               *
 *                                                                      *
 * Developed by: Jeff Xie                                               *
 * Date: 2021-11-04                                                     *
 ************************************************************************/

#ifndef AVATARVISION_IMU_INTEGRATE_MADGWICK_HPP_
#define AVATARVISION_IMU_INTEGRATE_MADGWICK_HPP_

#include <cmath>
#include <vector>

#include "quaternion.h"

namespace utils {
/**
 * @brief Implementation of Madgwick's IMU and AHRS algorithms
 * Be careful with the coordinate of input and the init quaternion setting
 * The tested one is x forward, y left, z up.
 * If your imu data is in different coordinate, change it first.
 *
 */
class MadgwickIntegrate {
public:
  /**
   * @brief Construct
   *
   */
  MadgwickIntegrate();

  /**
   * @brief Construct
   *
   * @param beta 2 * proportional gain
   */
  MadgwickIntegrate(double beta);

  /**
   * @brief get the current rotation quaternion
   *
   * @return Quaternion
   */
  quaternion get() const;

  /**
   * @brief calculate the current rotation quaternion
   *
   * @param gx gyro x radian
   * @param gy gyro y radian
   * @param gz gyro z radian
   * @param ax accelerometer x gravity
   * @param ay accelerometer y gravity
   * @param az accelerometer z gravity
   * @param mx magnetometer x uT
   * @param my magnetometer y uT
   * @param mz magnetometer z uT
   */
  void update(float gx, float gy, float gz, float ax, float ay, float az,
              float mx, float my, float mz, float dt);

  /**
   * @brief calculate the current rotation quaternion
   *
   * @param gx gyro x radian
   * @param gy gyro y radian
   * @param gz gyro z radian
   * @param ax accelerometer x gravity
   * @param ay accelerometer y gravity
   * @param az accelerometer z gravity
   * @param dt delta time of the frame
   */
  void updateIMU(float gx, float gy, float gz, float ax, float ay, float az,
                 float dt);

  void reset();

  static float invSqrt(float x);

private:
  quaternion q = quaternion(); //! output rotation
  double beta = 0.1;           //! 2 * proportional gain
};

} // namespace utils

#endif // AVATARVISION_IMU_INTEGRATE_MADGWICK_HPP_