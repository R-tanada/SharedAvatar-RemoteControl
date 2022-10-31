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

#ifndef __VARIABLEMANAGER__
#define __VARIABLEMANAGER__

#include "JSONUtils.h"
#include <string>

#include "ISingleton.h"

namespace utils {

class VariableManagerImpl;
class AvaUtils_API VariableManager : public ISingleton<VariableManager> {
private:
  VariableManagerImpl *_impl;

public:
  VariableManager(/* args */);
  virtual ~VariableManager();

  /**
   * @brief Set a Variable value, the name doesn't have to start with $
   * character
   *
   * @param name
   * @param value
   */
  void SetVariable(const std::string &name, const std::string &value);

  /**
   * @brief Get the Variable object "only" if the name starts with $
   * character, else it will return "name" value. If it starts with $ but not
   * found, then will return defaultVal
   *
   * @param name
   * @param defaultVal
   * @return std::string
   */
  const std::string &GetVariable(const std::string &name,
                                 const std::string &defaultVal = "");

  /**
   * @brief removes a variable from the manager,  the name doesn't have to
   * start with $
   *
   * @param name
   */
  void RemoveVariable(const std::string &name);

  /**
   * @brief removes a variable from the manager,  the name "must" start with $
   *
   * @param name
   */
  bool IsVariableExists(const std::string &name);

  /**
   * @brief Remove all variables
   *
   */
  void ClearVariables();

  bool LoadFromFile(const std::string &path);
  bool Load(JSONObject &tree);

  std::string ExportToJson();
};

#define gVariableManager utils::VariableManager::getInstance()

} // namespace utils

#endif //__VARIABLEMANAGER__