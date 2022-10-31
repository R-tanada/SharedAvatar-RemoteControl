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

#ifndef __GSTPIPELINEHANDLER__
#define __GSTPIPELINEHANDLER__

#include <mutex>
#include <string>

#include "flow/avatarflow_config.h"
#include "utils/ListenerContainer.h"
#include "utils/atypes.h"

namespace avatarflow {
namespace media {

class GstPipelineHandlerImpl;
class GstPipelineHandler;

class IPipelineListener {
public:
  virtual ~IPipelineListener() {}
  virtual void OnPipelineReady(GstPipelineHandler *p) {}
  virtual void OnPipelinePlaying(GstPipelineHandler *p) {}
  virtual void OnPipelinePreStop(GstPipelineHandler *p) {}
  virtual void OnPipelineStopped(GstPipelineHandler *p) {}
  virtual void OnPipelineClosed(GstPipelineHandler *p) {}
  virtual void OnPipelineError(GstPipelineHandler *p) {}
  virtual void OnPipelineWarning(GstPipelineHandler *p) {}
  virtual void OnPipelineEOS(GstPipelineHandler *p) {}
  virtual void OnPipelineElementMessage(GstPipelineHandler *p,
                                        const std::string &elem, void *msg) {}
};

class AvaFlow_API GstPipelineHandler
    : public utils::ListenerContainer<IPipelineListener *> {
protected:
  GstPipelineHandlerImpl *m_data;
  std::recursive_mutex _stateMutex;
  std::recursive_mutex _dataMutex;

public:
  DECLARE_FIRE_METHOD(OnPipelineReady, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelinePlaying, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelinePreStop, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineStopped, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineClosed, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineError, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineWarning, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineEOS, (GstPipelineHandler * p), (p));
  DECLARE_FIRE_METHOD(OnPipelineElementMessage,
                      (GstPipelineHandler * p, const std::string &elem,
                       void *msg),
                      (p, elem, msg));

public:
  GstPipelineHandler(const std::string &name);
  virtual ~GstPipelineHandler();

  // set isMasterClock to true if
  virtual bool CreatePipeline(const std::string &pipeline);
  virtual bool CreatePipeline(); // Create using last time pipeline string
  virtual bool SetPaused(bool p, bool force = false);
  virtual bool Stop();
  virtual bool IsLoaded();
  virtual bool IsPlaying();
  virtual bool IsPaused();
  virtual bool Close();
  void SendEOS();

  virtual bool QueryLatency(bool &isLive, utils::ulong &minLatency,
                            utils::ulong &maxLatency);

  virtual void SetClockBaseTime(utils::ulong baseTime);
  virtual utils::ulong GetClockBaseTime();

  void SetClockAddr(const std::string &host, int port);
  int GetClockPort();

  void SetPipeline(void *p);
  void *GetPipeline();

  std::recursive_mutex &GetMutex(int i = 0);
};

} // namespace media
} // namespace avatarflow

#endif // GstPipelineHandler_h__
