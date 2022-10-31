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

#ifndef __THREADPOOLMANAGER__
#define __THREADPOOLMANAGER__

#include "avataros_config.h"
#include <utils/ISingleton.h>
#include <utils/ListenerContainer.h>

namespace avatarOS {

class ThreadPool;
class IThreadJob;

class IThreadPoolManagerListener {
public:
  virtual void OnJobStarted(int threadID, IThreadJob *job) {}
  virtual void OnJobFinished(int threadID, IThreadJob *job, bool result) {}
};

class ThreadPoolManagerImpl;

class AvaOS_API ThreadPoolManager
    : public utils::ISingleton<ThreadPoolManager>,
      public utils::ListenerContainer<IThreadPoolManagerListener *> {
private:
  ThreadPoolManagerImpl *_impl;

public:
  DECLARE_FIRE_METHOD(OnJobStarted, (int threadID, IThreadJob *job),
                      (threadID, job));
  DECLARE_FIRE_METHOD(OnJobFinished,
                      (int threadID, IThreadJob *job, bool result),
                      (threadID, job, result));

protected:
  friend class ThreadPool;

  void _OnJobStarted(ThreadPool *pool, IThreadJob *job);
  void _OnJobFinished(ThreadPool *pool, IThreadJob *job, bool result);

public:
  ThreadPoolManager(/* args */);
  virtual ~ThreadPoolManager();

  /**
   * @brief Set the number of parallely running threads (thread pool size)
   *
   * @param count
   * @return Success if size of the pool was changed, otherwise fails
   */
  bool SetThreadPoolSize(int size);

  /**
   * @brief Get the Thread Pool Size
   *
   * @return int
   */
  int GetThreadPoolSize();

  /**
   * @brief Get the Active Job if its running in a pool
   *
   * @return IThreadJob*
   */
  IThreadJob *GetActiveJob(int pool);

  /**
   * @brief Get the number of jobs in a pool
   *
   * @param pool
   * @return int
   */
  int GetJobCount(int pool);
  /**
   * @brief Get the Job From a thread Pool using its index
   *
   * @param pool
   * @param index
   * @return IThreadJob*
   */
  IThreadJob *GetJobFromPool(int pool, int index);

  /**
   * @brief Add a job to a pool, if pool was -1 then it will automatically be
   * assigned to the smallest pool
   *
   * @param job
   */
  bool AddJob(IThreadJob *job, int pool = -1, bool deleteWhenDone = false);

  /**
   * @brief Remove a job if it was not started yet
   *
   * @return true if the job was removed
   * @return false if the job was not found or already started
   */
  bool RemoveJob(IThreadJob *job);

  /**
   * @brief Start the main thread to monitor the pools and executed them
   *
   */
  void StartThreadPool();

  /**
   * @brief Stop all the pool
   *
   */
  void StopThreadPool();
};

#define gThreadPoolManager avatarOS::ThreadPoolManager::getInstance()

} // namespace avatarOS

#endif //__THREADPOOLMANAGER__