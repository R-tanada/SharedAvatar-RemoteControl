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

#ifndef __IPROPERTY__
#define __IPROPERTY__

#include "GUID.h"

namespace utils {

enum EPropertieAccess { EPA_READ, EPA_WRITE, EPA_RW };

enum EPropertyFlags {
  EPF_Read = BIT(0),
  EPF_Write = BIT(1),
  EPF_Serialize = BIT(2),
  EPF_Modify = BIT(3)
};

class PropertySet;
class IPropertyVistor;
class EnumInfo;

enum EPropertyBasicType { EPBT_Basic, EPBT_Struct, EPBT_Enum, EPBT_List };

class AvaUtils_API IProperty {
protected:
  std::string m_name;
  std::string m_info;
  const EnumInfo *m_enum;

  EPropertyBasicType m_type;

  uint m_flags;

  utils::GUID m_propertyType;

  std::vector<std::string> m_itemList;
  EPropertieAccess m_access;

public:
  IProperty(const std::string &name, EPropertyBasicType type,
            const utils::GUID &propType, const std::string &info = aT(""));

  inline void SetFlag(EPropertyFlags f, bool set) {
    if (set)
      m_flags |= (uint)f;
    else
      m_flags &= ~(uint)f;
  }
  inline bool GetFlag(EPropertyFlags f) const { return m_flags & (uint)f; }

  const utils::GUID &GetPropertyType() const;

  const std::string &GetName() const;
  const std::string &GetInfo() const;
  void SetInfo(const std::string &info);

  EPropertieAccess GetAccessMode() const;

  virtual std::string ToString(PropertySet *object) const = 0;
  virtual bool Parse(PropertySet *reciver, const std::string &str) = 0;

  virtual bool IsDefault(PropertySet *reciver) const = 0;
  virtual EPropertyBasicType GetType() const { return m_type; }

  virtual void OnVisit(IPropertyVistor *visitor);

  void SetItemList(const std::vector<std::string> &items);
  const std::vector<std::string> &GetSupportedItems() const;

  void SetEnum(const EnumInfo *e);
  const EnumInfo *GetEnum() const;

  virtual bool CheckIsValidValue(const std::string &v);
};

} // namespace utils

#endif