
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

#ifndef __CAPABILITIES__
#define __CAPABILITIES__

#include "JSONUtils.h"

#include "ListenerContainer.h"
#include "avatarutils_config.h"

namespace utils {

class Capabilities;
typedef std::string CapabilityType;

class ICapabilitiesListener {
   public:
   virtual ~ICapabilitiesListener(){}
    virtual void OnCapabilityChanged(Capabilities *cap, const std::string &id,
                                     const CapabilityType &oldValue,
                                     const CapabilityType &newValue) {}
    virtual void OnCapabilitySynced(Capabilities *cap) {}
    virtual void OnCapabilityCleared(Capabilities *cap) {}
    virtual void OnCapabilityLoaded(Capabilities *cap) {}
};

class AvaUtils_API Capabilities
    : public ListenerContainer<ICapabilitiesListener *> {
   protected:
    JSONObject _caps;
    // typedef std::map<std::string, CapabilityType> CapMap;
    // CapMap _caps;
    std::string _syncPath;
    bool _autoSync;

    DECLARE_FIRE_METHOD(OnCapabilityChanged,
                        (Capabilities * cap, const std::string &id,
                         const CapabilityType &oldValue,
                         const CapabilityType &newValue),
                        (cap, id, oldValue, newValue));
    DECLARE_FIRE_METHOD(OnCapabilitySynced, (Capabilities * cap), (cap));
    DECLARE_FIRE_METHOD(OnCapabilityCleared, (Capabilities * cap), (cap));
    DECLARE_FIRE_METHOD(OnCapabilityLoaded, (Capabilities * cap), (cap));

   public:
    Capabilities();
    ~Capabilities();

    bool HasCapability(const std::string &cap) const;
    const JSONObject *GetCapability( const std::string &cap) const;
    void SetCapability(const std::string &cap, const CapabilityType &value);
    const JSONObject &GetCapabilities() const {
        return _caps;
    }

    int GetCapability(const std::string &cap, int defaultValue);
    float GetCapability(const std::string &cap, float defaultValue);
    bool GetCapability(const std::string &cap, bool defaultValue);
    std::string GetCapability(const std::string &cap,
                              const std::string &defaultValue);

    void Clear();

    const JSONObject &GetValues(
        const std::string &name) const;  // export values as property tree

    bool LoadFromFile(const std::string &path);
    bool Load(JSONObject &tree);

    std::string ExportToJson();

    void SetSyncPath(const std::string &path);
    const std::string &GetSyncPath() const;
    void SetAutoSync(bool sync);
    bool IsAutoSync() const;
    bool Sync();

   public:
};

#define DECLARE_CAPABILITY_NAME(name) static const std::string CAP_##name;
#define IMPLEMENT_CAPABILITY_NAME(class, name) \
    const std::string class ::CAP_##name(#name);

}  // namespace utils

#endif  //__CAPABILITIES__
