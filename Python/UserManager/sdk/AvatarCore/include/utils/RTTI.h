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

#ifndef __RTTI__
#define __RTTI__

#include "GUID.h"

namespace utils {

class AvaUtils_API RTTI {
  std::string _name;
  RTTI *_parent;
  utils::GUID _guid;

public:
  RTTI(const char *name, RTTI *parent) : _parent(parent) {
    _name = name;
    _guid = _name;
  };

  const utils::GUID &IGetGUID() const { return _guid; }

  const std::string &GetTypeName() const { return _name; }

  bool IsKindOf(const RTTI *other) const {
    if (this == other)
      return true;
    RTTI *p = _parent;
    while (p) {
      if (p == other)
        return true;
      p = p->_parent;
    }
    return false;
  }
  bool IsKindOf(const std::string &other) const {
    if (_name == other)
      return true;
    RTTI *p = _parent;
    while (p) {
      if (p->_name == other)
        return true;
      p = p->_parent;
    }
    return false;
  }

  RTTI *GetParent() { return _parent; }
};

#define DECLARE_RTTI                                                           \
protected:                                                                     \
  static utils::RTTI ms_RTTI_def;                                              \
                                                                               \
public:                                                                        \
  static const std::string &GetClassType() {                                   \
    return ms_RTTI_def.GetTypeName();                                          \
  }                                                                            \
  virtual const std::string &GetObjectType() const {                           \
    return ms_RTTI_def.GetTypeName();                                          \
  }                                                                            \
  static bool IsKindOf(const utils::RTTI *other) {                             \
    return ms_RTTI_def.IsKindOf(other);                                        \
  }                                                                            \
  static const utils::RTTI *GetClassRTTI() { return &ms_RTTI_def; }            \
  virtual const utils::RTTI *GetObjectRTTI() const { return &ms_RTTI_def; }    \
  template <class T> T *TryCast() {                                            \
    if (ms_RTTI_def.IsKindOf(T::GetClassRTTI()))                               \
      return (T *)this;                                                        \
    else                                                                       \
      return (T *)0;                                                           \
  }

#define IMPLEMENT_RTTI(type, parent)                                           \
  utils::RTTI type::ms_RTTI_def(#type, &parent::ms_RTTI_def);
#define IMPLEMENT_ROOT_RTTI(type) utils::RTTI type::ms_RTTI_def(#type, 0);

} // namespace utils

#endif //__RTTI__