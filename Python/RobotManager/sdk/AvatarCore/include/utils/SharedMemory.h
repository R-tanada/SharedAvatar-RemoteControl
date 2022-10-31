

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

#ifndef __SHAREDMEMORY__
#define __SHAREDMEMORY__

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>

#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {
class SharedMemoryImpl;

typedef boost::interprocess::
    interprocess_upgradable_mutex  // interprocess_mutex  //
                                   // interprocess_upgradable_mutex
        shmem_mutex_type;
typedef boost::interprocess::
    interprocess_condition  // interprocess_mutex  //
                            // interprocess_upgradable_mutex
        shmem_condition_type;

typedef boost::interprocess::scoped_lock<
    shmem_mutex_type>  // scoped_lock<shmem_mutex_type>
                       // // sharable_lock
    shmem_lock_type;

class AvaUtils_API SharedMemory {
   private:
    SharedMemoryImpl *_impl;

   public:
    SharedMemory(/* args */);
    ~SharedMemory();

    bool Create(const std::string &name, int size);  // create new memory
    bool Open(const std::string &name);              // open existing memory
    void Close();

    const std::string &GetName() const;

    bool IsMaster();
    bool IsOpen();

    uint GetSize();

    utils::byte *GetAddress();

    utils::byte *LockRegin(int start, int length);
    void UnlockRegin(utils::byte *data);

    void Lock();
    void Unlock();

    shmem_mutex_type *Mutex();

    bool Wait();        // interprocess wait
    void TriggerAll();  // trigger all waiting processes

    static bool IsSharedMemoryCreated(const std::string &name);
    static int GetSharedMemorySize(const std::string &name);
    static bool CloseSharedMemory(const std::string &name);
};

}  // namespace utils

#endif  //__SHAREDMEMORY__