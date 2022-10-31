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

#ifndef __RINGBUFFER__
#define __RINGBUFFER__

#include "atypes.h"
#include "avatarutils_config.h"

namespace utils {
class AvaUtils_API RingBuffer {
   protected:
    utils::byte* _ptr;
    int _length;
    int _top;
    int _readingOffset;

    bool _destroy;

   public:
    RingBuffer();
    virtual ~RingBuffer();

    void Init(int length);
    void SetBufferPtr(utils::byte* ptr, int length, bool autoRemove = false);
    void SetOffsets(int readingOffset, int top) {
        _readingOffset = readingOffset;
        _top = top;
    }

    int CalculateAvailbleBytes();

    int WriteData(const utils::byte* data, int len);
    int ReadData(utils::byte* data, int len);
};
}  // namespace utils

#endif  //__RINGBUFFER__