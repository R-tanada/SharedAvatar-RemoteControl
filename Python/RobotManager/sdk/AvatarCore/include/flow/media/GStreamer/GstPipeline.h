
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

#ifndef __GSTPIPELINE__
#define __GSTPIPELINE__

#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {
namespace media {

class GstPipelineHandler;
class GstPipelineImpl;
// Creates and manages gstreamer pipeline
class AvaFlow_API GstPipeline {
private:
  GstPipelineImpl *_impl;

public:
  GstPipeline(GstPipelineHandler *handler);
  ~GstPipeline();

  void SetHandler(GstPipelineHandler *handler);
  GstPipelineHandler *GetHandler();
  bool CreatePipeline(const std::string &pipeline);

  void *GetGstPipelinePtr(); // return gstreamer GstElement* pipeline

  void *GetElementByName(const std::string &name);
  bool SetElementProperty(const std::string &name, const std::string &prop,
                          int value);

  bool Start(bool force = false);
  bool Pause(bool force = false);
  bool Stop();
  bool Close();
  bool IsPlaying();
  bool IsPaused();
  bool IsStopped();

  /**
   * @brief Restart the pipeline, handles stopping and playing back
   *
   * @return true
   * @return false
   */
  bool Restart(bool hard = false);

  void SendEOS();
};

} // namespace media
} // namespace avatarflow

#endif //__GSTPIPELINE__
