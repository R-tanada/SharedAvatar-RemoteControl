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

#ifndef __ENUMMANAGER__
#define __ENUMMANAGER__

#include <map>

#include "ISingleton.h"

namespace utils {

typedef std::map<std::string, int> EnumNameValMap;
typedef std::map<int, std::string> EnumValNameMap;

class AvaUtils_API EnumInfo {
   public:
    std::string name;
    EnumNameValMap NameVal;
    EnumValNameMap ValName;

    bool ContainsValue(const std::string &v) const;
    bool ContainsValue(int v) const;

    const std::string &ToString(int v) const;
    int Parse(const std::string &v) const;
};
typedef std::map<std::string, EnumInfo *> EnumInfoMap;

class AvaUtils_API EnumManager : public ISingleton<EnumManager> {
   private:
    EnumInfoMap m_enumInfo;
    EnumInfo *m_currInfo;

    const std::string m_dummyString;

   public:
    EnumManager();
    virtual ~EnumManager();

    void startEnum(const std::string &name);
    void addValue(const std::string &name, int val);
    void endEnum();

    void removeEnum(const std::string &name);

    void clear();

    int getValue(const std::string &e, const std::string &v);
    const std::string &getName(const std::string &e, int v);
    const EnumInfo *getEnumInfo(const std::string &e);
};

}  // namespace utils

#define StartRegisterEnum(e) mray::EnumManager::getInstance().startEnum(mT(#e))
#define RegisterEnumValue(v) \
    mray::EnumManager::getInstance().addValue(mT(#v), (int)v)
#define RegisterEnumValueA(alias, v) \
    mray::EnumManager::getInstance().addValue(alias, (int)v)
#define EndRegisterEnum() mray::EnumManager::getInstance().endEnum()

#endif  //___EnumManager___
