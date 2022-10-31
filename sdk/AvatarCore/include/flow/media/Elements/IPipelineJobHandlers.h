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

#ifndef __IPIPELINEJOBHANDLERS__
#define __IPIPELINEJOBHANDLERS__

#include "OS/IThreadJob.h"
#include "flow/avatarflow_config.h"
#include <string>

namespace avatarflow {
namespace media {

class AvaFlow_API IPipelineJobHandlers {
private:
protected:
  class CreatePipelineJob : public avatarOS::IThreadJob {
  public:
    IPipelineJobHandlers *owner;
    CreatePipelineJob(IPipelineJobHandlers *o);
    virtual bool Execute();
  } * _createPipelineJob;
  class PlayJob : public avatarOS::IThreadJob {
  public:
    IPipelineJobHandlers *owner;
    PlayJob(IPipelineJobHandlers *o);
    virtual bool Execute();
  } * _playJob;
  class DestroyJob : public avatarOS::IThreadJob {
  public:
    IPipelineJobHandlers *owner;
    DestroyJob(IPipelineJobHandlers *o);
    virtual bool Execute();
  } * _destroyJob;

public:
  IPipelineJobHandlers(/* args */);
  virtual ~IPipelineJobHandlers();
  void _DestroyJobs();

  void _InitJobs();
  void _WaitForJobs();

  virtual bool _CreateJob(bool threaded, int jobIndex);
  virtual bool _PlayJob(bool threaded, int jobIndex);
  virtual bool _DestroyJob(bool threaded, int jobIndex);

  virtual const std::string &_GetOwnerName() = 0;
  virtual bool _CreatePipelineThreaded();
  virtual bool _PlayThreaded();
  virtual bool _DestroyThreaded();
};

} // namespace media
} // namespace avatarflow

#endif //__PIPELINEJOBHANDLERS__