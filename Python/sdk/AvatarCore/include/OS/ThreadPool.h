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

#ifndef __THREADPOOL__
#define __THREADPOOL__

#include "avataros_config.h"

namespace avatarOS {

class IThreadJob;

class ThreadPoolImpl;
class AvaOS_API ThreadPool {
private:
  ThreadPoolImpl *_impl;

protected:
  friend class ThreadPoolImpl;
  void _ProcessJob(IThreadJob *job);

public:
  ThreadPool(/* args */);
  virtual ~ThreadPool();

  void SetPoolID(int id);
  int GetPoolID();

  void AddJob(IThreadJob *job, bool deleteWhenDone = false);
  int GetJobCount();
  int GetProcessingJobCount();
  IThreadJob *GetJob(int index);
  IThreadJob *GetActiveJob();
  bool RemoveJob(IThreadJob *job);

  void StartPool();
  void StopPool();
};

} // namespace avatarOS

#endif //__THREADPOOL__