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

#ifndef __ITHREADJOB__
#define __ITHREADJOB__

#include <condition_variable>
#include <mutex>
#include <string>

namespace avatarOS {
/**
 * @brief a job to be executed by the thread manager, make sure to keep the job
 * relatively short and not running in an infinite loop as it will block other
 * jobs in the same queue
 *
 */

class ThreadPool;
class ThreadPoolImpl;
class IThreadJob {
private:
  std::string _identifier;
  bool _isExecuting;
  bool _interrupted;
  bool _inQueue;

  std::condition_variable _jobCond;
  std::mutex _jobMutex;

protected:
  friend class ThreadPool;
  friend class ThreadPoolImpl;
  bool _ExcuteJob() {
    if (_isExecuting || _interrupted)
      return false;
    _isExecuting = true;
    bool ret = Execute();
    _isExecuting = false;
    _jobCond.notify_all();
    return ret;
  }

  void _SetInQueue(bool inQueue) { _inQueue = inQueue; }

public:
  IThreadJob() : _identifier("N/A") {}
  IThreadJob(const std::string &id)
      : _identifier(id), _isExecuting(false), _interrupted(false),
        _inQueue(false) {}
  virtual ~IThreadJob() {}

  /**
   * @brief Get a string representing this job, optional
   *
   * @return std::string
   */
  virtual const std::string &GetIdentifier() { return _identifier; }
  /**
   * @brief Function will be called by the thread manager to execute it
   *
   * @return true
   * @return false
   */
  virtual bool Execute() = 0;

  virtual bool IsExecuting() { return _isExecuting; }

  /**
   * @brief Called by the pool to interrupt the execution
   *
   * @return true
   * @return false
   */
  virtual void Interrupt() { _interrupted = true; }
  virtual bool IsInterrupted() { return _interrupted; }
  virtual void ResetInterrupt() { _interrupted = false; }

  void WaitUntilDone() {
    std::unique_lock<std::mutex> lock(_jobMutex);
    if (!_isExecuting)
      return;
    _jobCond.wait(lock);
  }

  bool IsInQueue() { return _inQueue; }
};

} // namespace avatarOS

#endif //__ITHREADJOB__