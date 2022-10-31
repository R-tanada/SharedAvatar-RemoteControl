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

#ifndef __JSONUTILS__
#define __JSONUTILS__

#include "atypes.h"
#include "avatarutils_config.h"

#ifdef USE_BOOST_JSON
#include <boost/property_tree/ptree.hpp>

typedef boost::property_tree::iptree JSONObject;
#else
#include <nlohmann/json.hpp>
typedef nlohmann::json JSONObject;
#endif

namespace utils {

/**
 * @brief Helper class to facilitate the use of JSON objects, parsing and
 * generating json strings
 *
 */
class AvaUtils_API JSONUtils {
public:
  /**
   * @brief Parse a json string from memory
   *
   * @param json
   * @param results
   * @return true
   * @return false
   */
  static bool SafeParseJSON(const std::string &json, JSONObject &results);

  /**
   * @brief Parse a json file
   *
   * @param path
   * @param results
   * @return true
   * @return false
   */
  static bool SafeParseJSONFromFile(const std::string &path,
                                    JSONObject &results);

  /**
   * @brief Generates a json string (dump string)
   *
   * @param json
   * @param result
   * @param pretty Will add tabs to the result string, keep false to save some
   * bytes
   * @return true
   * @return false
   */
  static bool GenerateJSON(const JSONObject &json, std::string &result,
                           bool pretty = false);

  /**
   * @brief returns a json member using cascaded path, e.g.:
   * Object.Subobject.member
   *
   * @param json json object
   * @param path path to the member in cascaded dot format
   * @param create If true, will create the path to the member
   * @return JSONObject*
   */
  static JSONObject *RecursiveGetMember(JSONObject &json,
                                        const std::string &path,
                                        bool create = false);

  static std::string GetStringValue(const JSONObject &json,
                                    const std::string &def = "");
  static utils::uint GetUIntValue(const JSONObject &json, utils::uint def = 0);
  static int GetIntValue(const JSONObject &json, int def = 0);
  static float GetFloatValue(const JSONObject &json, float def = 0);
  static bool GetBoolValue(const JSONObject &json, bool def = false);
};
} // namespace utils

#endif //__JSONUTILS__