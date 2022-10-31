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

#ifndef __TYPEDPROPERTY__
#define __TYPEDPROPERTY__

#include "IProperty.h"

namespace utils {

#define DECLARE_PROPERTY_TYPE(type, dataType, SPECS)                           \
  class SPECS PropertyType##type : public utils::IProperty {                   \
  protected:                                                                   \
    dataType m_default;                                                        \
                                                                               \
  public:                                                                      \
    static PropertyType##type instance;                                        \
    PropertyType##type();                                                      \
    virtual dataType GetValue(utils::PropertySet *object) const;               \
    virtual bool SetValue(utils::PropertySet *object, const dataType &v);      \
    virtual std::string ToString(utils::PropertySet *object) const;            \
    virtual bool Parse(utils::PropertySet *reciver, const std::string &str);   \
    virtual const dataType &GetDefaultValue() const { return m_default; }      \
    virtual bool IsDefault(utils::PropertySet *reciver) const;                 \
  };

#define IMPLEMENT_PROPERTY_TYPE_HEADER(type, ObjectType, dataType, Name,       \
                                       proptype, Description, Default)         \
  ObjectType::PropertyType##type::PropertyType##type()                         \
      : IProperty(Name, proptype, utils::GUID(#dataType), Description),        \
        m_default(Default) {}
//:TypedProperty(Name,proptype,utils::GUID(#dataType),Description)

#define IMPLEMENT_PROPERTY_TYPE_GENERIC(type, ObjectType, dataType,            \
                                        SetFunction, GetFunction,              \
                                        toStringFunc, parseFunc, useCheck)     \
  ObjectType::PropertyType##type ObjectType::PropertyType##type::instance;     \
                                                                               \
  dataType ObjectType::PropertyType##type::GetValue(                           \
      utils::PropertySet *object) const {                                      \
    ObjectType *o = dynamic_cast<ObjectType *>(object);                        \
    if (!o)                                                                    \
      return m_default;                                                        \
    return o->GetFunction();                                                   \
  }                                                                            \
  bool ObjectType::PropertyType##type::SetValue(utils::PropertySet *object,    \
                                                const dataType &v) {           \
    ObjectType *o = dynamic_cast<ObjectType *>(object);                        \
    if (!o)                                                                    \
      return false;                                                            \
    return o->SetFunction(v);                                                  \
  }                                                                            \
  std::string ObjectType::PropertyType##type::ToString(                        \
      utils::PropertySet *object) const {                                      \
    return toStringFunc(GetValue(object));                                     \
  }                                                                            \
  bool ObjectType::PropertyType##type::Parse(utils::PropertySet *reciver,      \
                                             const std::string &str) {         \
    if (useCheck) {                                                            \
      if (!CheckIsValidValue(str))                                             \
        return false;                                                          \
    }                                                                          \
    return SetValue(reciver, parseFunc(str));                                  \
  }                                                                            \
  bool ObjectType::PropertyType##type::IsDefault(utils::PropertySet *reciver)  \
      const {                                                                  \
    if (GetValue(reciver) == GetDefaultValue())                                \
      return true;                                                             \
    return false;                                                              \
  }

#define DECLARE_SETGET_PROP(Name, dataType)                                    \
  virtual bool Set##Name(const dataType &v);                                   \
  virtual const dataType &Get##Name() const;

// this will declare a property and add set/get functions
#define DECLARE_PROPERTY_SETGET(Name, dataType)                                \
  DECLARE_PROPERTY_TYPE(Name, dataType, )                                      \
  DECLARE_SETGET_PROP(Name, dataType)

#define IMPLEMENT_SETGET_PROP(object, Name, dataType, value, default, setter,  \
                              getter, description)                             \
  bool object::Set##Name(const dataType &v) {                                  \
    value = v;                                                                 \
    return true;                                                               \
  }                                                                            \
  const dataType &object::Get##Name() const { return value; }                  \
  IMPLEMENT_PROPERTY_TYPE_HEADER(Name, object, dataType, aT(#Name),            \
                                 utils::EPBT_Basic, description, default);     \
  IMPLEMENT_PROPERTY_TYPE_GENERIC(Name, object, dataType, Set##Name,           \
                                  Get##Name, getter, setter, false);

} // namespace utils

#endif // TypedProperty_h__
