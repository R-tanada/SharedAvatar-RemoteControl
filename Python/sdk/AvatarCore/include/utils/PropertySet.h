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

#ifndef __PropertySet__
#define __PropertySet__

#include "IDelegate.h"
#include "IDelegateContainer.h"
#include "IProperty.h"
#include "JSONUtils.h"
#include "RTTI.h"
#include "avatarutils_config.h"
#include "base.h"

namespace utils {

class RTTI;
class PropertieDictionary;

typedef DelegateEvent2<IObject *, PVOID> ObjectEvent;
#define CreateObjectDelegate(ObjectType, obj, Delegate)                        \
  newClassDelegate2<ObjectType, void, IObject *, void *>(                      \
      #Delegate, obj, &ObjectType::Delegate)

typedef std::map<std::string, IProperty *> TPropertieList;
typedef TPropertieList::iterator TPropertieListIT;

typedef std::map<std::string, ObjectEvent *> TEventsList;
typedef TEventsList::iterator TEventsListIT;

typedef std::map<std::string, PropertieDictionary *> TSubPropertyDictionary;
typedef TSubPropertyDictionary::iterator TSubPropertyDictionaryIT;

class PropertyGroup {
public:
  PropertyGroup() {}
  ~PropertyGroup() { properties.clear(); }
  std::string name;
  TPropertieList properties;
};

typedef std::map<std::string, PropertyGroup> TPropertieGroupList;

class AvaUtils_API PropertieDictionary {
protected:
  TPropertieList m_properties;
  TPropertieGroupList m_groups;

  TSubPropertyDictionary m_subDics;

  const RTTI *m_rtti;

public:
  PropertieDictionary(const RTTI *rtti);
  virtual ~PropertieDictionary();

  const utils::GUID *GetPropertyType(const std::string &name);
  size_t GetPropertiesCount();
  const TPropertieList &GetProperties();

  const TPropertieGroupList &GetGroups();

  IProperty *GetPropertie(const std::string &name);

  const RTTI *GetRTTI() const { return m_rtti; }

  void AddPropertie(IProperty *prop, const std::string &group = aT("Default"));

  void AddSubDictionary(const std::string &name, PropertieDictionary *set);
  PropertieDictionary *GetSubDictionary(const std::string &name);
  const TSubPropertyDictionary &GetSubDictionaries();

  PropertyGroup *GetGroup(const std::string &name);

  std::string GetPropertieInfo(const std::string &name);
  void SetPropertieInfo(const std::string &name, const std::string &info);

  virtual void OnVisit(IPropertyVistor *visitor);
};

class AvaUtils_API PropertySet : public IDelegateContainer {
  DECLARE_RTTI

public:
  typedef std::map<std::string, PropertySet *> SubsetMap;

private:
  typedef std::map<std::string, GCPtr<PropertieDictionary>>
      PropertyDictionaryMap;
  static PropertyDictionaryMap m_dictionaries;

protected:
  typedef std::map<std::string, std::list<std::string>> EventDelegateMap;
  EventDelegateMap m_eventDelegMap;

  typedef std::list<IDelegateContainer *> EventContainerList;
  EventContainerList m_eventContainer;
  PropertieDictionary *m_dictionary;
  SubsetMap m_subSets;
  TEventsList m_events;

  void _OnEventFired(ObjectEvent *name, IObject *p1, PVOID p2);

public:
  static bool CreateDictionary(const std::string &name, const RTTI *obj,
                               PropertieDictionary **dic);
  static PropertieDictionary *GetDictionary(const std::string &name);

  PropertySet();
  virtual ~PropertySet();

  bool GetOrCreateDictionary(PropertieDictionary **dic);
  PropertieDictionary *GetDictionary();

  std::string GetPropertyValue(const std::string &name);
  void SetPropertyValue(const std::string &name, const std::string &v);

  void AddSubSet(const std::string &name, PropertySet *s);
  PropertySet *GetSubSet(const std::string &name);
  const SubsetMap &GetSubSets();

  void AddEvent(ObjectEvent *e);
  ObjectEvent *GetEvent(const std::string &name);
  void FireEvent(const std::string &name, IObject *p1, PVOID p2);
  size_t GetEventsCount();
  const TEventsList &GetEvents();

  void AddEventContainer(IDelegateContainer *d);
  void RemoveEventContainer(IDelegateContainer *d);
  void ClearEventContainers();
  void AddEventDelegatePair(const std::string &event,
                            const std::string &target);
  void RemoveEventDelegatePair(const std::string &event,
                               const std::string &target);
  void ClearDelegatesForEvent(const std::string &event);

  // virtual void loadFromXML(xml::XMLElement *elem);
  // virtual void exportToXML(xml::XMLElement *elem);

  virtual void OnVisit(IPropertyVistor *visitor);

  virtual void LoadSettings(const JSONObject &settings);
};

} // namespace utils

#endif