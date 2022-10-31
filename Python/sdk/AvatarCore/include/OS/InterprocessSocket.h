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

#ifndef __INTERPROCESSSOCKET__
#define __INTERPROCESSSOCKET__

#include <string>

namespace avatarOS {

class InterprocessSocket {
   private:
    int _socketID;

   public:
    InterprocessSocket(/* args */);
    ~InterprocessSocket();

    virtual bool Open(const std::string &path);
    virtual void Close();
    virtual bool IsConnected();
    virtual int Send(const std::string &data);
};

}  // namespace avatarOS

#endif  //__IINTERPROCESSSOCKET__