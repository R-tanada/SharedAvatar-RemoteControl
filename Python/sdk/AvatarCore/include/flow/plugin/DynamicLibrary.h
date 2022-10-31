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

#ifndef __DYNAMICLIBRARY__
#define __DYNAMICLIBRARY__

#include <string>

#include "flow/avatarflow_config.h"

namespace avatarflow {

class DynamicLibraryImpl;

/**
 * @brief This class encapsulates dynamic library loading and access to member
 * functions exported in the library. It supports windows/mac osx/linux systems.
 *
 */
class AvaFlow_API DynamicLibrary {
private:
  DynamicLibraryImpl *_impl;

public:
  DynamicLibrary(/* args */);
  ~DynamicLibrary();

  /**
   * @brief Load a shared library provided the path (relative or abstract)
   *
   * @param path Path to the library
   * @return true
   * @return false
   */
  bool Load(const std::string &path);

  /**
   * @brief Releases the loaded library, valid if all members of the library
   * have been released before hand
   *
   */
  void Unload();

  /**
   * @brief Whether or not the library is loaded
   *
   * @return true
   * @return false
   */
  bool IsLoaded() const;

  const std::string &GetLibraryName();

  /**
   * @brief Retrives a pointer to a function member of the library, must be
   * casted to the type of the function
   *
   * @param name
   * @return void* Null if the symbol is not found
   */
  void *GetSymbolPointer(const char *name) const;
};

} // namespace avatarflow

#endif //__DYNAMICLIBRARY__