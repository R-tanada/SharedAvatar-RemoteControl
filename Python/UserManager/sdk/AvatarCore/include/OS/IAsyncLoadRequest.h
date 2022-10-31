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

#ifndef __IASYNCLOADREQUEST__
#define __IASYNCLOADREQUEST__

namespace avatarOS {

class IAsyncLoadRequest {
private:
protected:
  bool m_loaded;

public:
  IAsyncLoadRequest() : m_loaded(false) {}
  virtual ~IAsyncLoadRequest() {}

  virtual bool StartLoading() = 0;
  virtual void OnFailedLoading() = 0;
  bool IsLoaded() { return m_loaded; }
};

} // namespace avatarOS

#endif