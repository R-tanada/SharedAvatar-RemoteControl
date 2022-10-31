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

#ifndef __base__
#define __base__

#include <memory>
#include <stdexcept>
#include <string>

#include "StackTrace.h"

namespace utils {

#define GCPtr std::shared_ptr

//#define MakeSharedPtr(type) class type;typedef type* type##Ptr;typedef const
// type*& type##CRef;
#define MakeSharedPtr(type)                  \
    class type;                              \
    typedef std::shared_ptr<type> type##Ptr; \
    typedef const std::shared_ptr<type> &type##CRef;

template <class T>
std::shared_ptr<T> SharedPtr(T *bdr) {
    return std::shared_ptr<T>(bdr);
}

#define FATAL_ERROR(condition, msg)                                         \
    if (condition) {                                                        \
        printf("Error at[ %s:%s]: %s\n Exiting...\n",                       \
               std::string(__FILE__).c_str(),                               \
               std::to_string(__LINE__).c_str(), std::string(msg).c_str()); \
        utils::StackTrace::PrintTrace(20);                                  \
        exit(0);                                                            \
    }
#define mT(v) (v)

}  // namespace utils

#endif  //__base__